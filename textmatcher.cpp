#include "textmatcher.h"
#include "./ui_textmatcher.h"
#include <QStyle>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QShortcut>
#include <QFileDialog>

// --- Local Constants ---
namespace {
    const QColor HighlightColor(100, 92, 17);
    const QColor CurrentHighlightColor(100, 52, 17);
}

// --- SearchOptions Implementation ---
TextMatcher::SearchOptions::SearchOptions(const QString &text, bool wholeWord, bool caseSensitive)
{
    const QString escaped = QRegularExpression::escape(text);
    const QString pattern = wholeWord
                                ? QStringLiteral("\\b") + escaped + QStringLiteral("\\b")
                                : escaped;

    patternOptions = caseSensitive
                         ? QRegularExpression::NoPatternOption
                         : QRegularExpression::CaseInsensitiveOption;

    regex = QRegularExpression(pattern, patternOptions);

    flags = caseSensitive
                ? QTextDocument::FindCaseSensitively
                : QTextDocument::FindFlags();
}

bool TextMatcher::SearchOptions::isValid() const
{
    return regex.isValid() && !regex.pattern().isEmpty();
}

// --- Constructor / Destructor ---
TextMatcher::TextMatcher(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextMatcher)
{
    ui->setupUi(this);
    setWindowIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));

    // Buttons
    connect(ui->findNextButton, &QPushButton::clicked, this, &TextMatcher::handleFindNext);
    connect(ui->findPrevButton, &QPushButton::clicked, this, &TextMatcher::handleFindPrevious);
    connect(ui->loadFileButton, &QPushButton::clicked, this, &TextMatcher::handleLoadFile);

    // Enter in the search field => Find Next
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &TextMatcher::handleFindNext);

    // Keyboard shortcuts: Up/Down = previous/next
    new QShortcut(QKeySequence(Qt::Key_Up), this, SLOT(handleFindPrevious()));
    new QShortcut(QKeySequence(Qt::Key_Down), this, SLOT(handleFindNext()));

    // Load default text
    loadTextFromFile(":/input.txt");
}

TextMatcher::~TextMatcher()
{
    delete ui;
}

// --- Cursor Utility ---

void TextMatcher::moveCursorToStart()
{
    QTextCursor c = ui->textEdit->textCursor();
    c.movePosition(QTextCursor::Start);
    ui->textEdit->setTextCursor(c);
}

void TextMatcher::moveCursorToEnd()
{
    QTextCursor c = ui->textEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(c);
}

void TextMatcher::clearTextSelection()
{
    QTextCursor c = ui->textEdit->textCursor();
    c.clearSelection();
    ui->textEdit->setTextCursor(c);
}

// --- Status Management ---

void TextMatcher::resetSearchState()
{
    clearTextSelection();
    m_totalMatches = 0;
    m_lastRegexPattern.clear();
    m_lastPatternOptions = QRegularExpression::NoPatternOption;
    ui->textEdit->setExtraSelections({});
    updateStatusLabel(0, 0);
}

void TextMatcher::updateStatusLabel(size_t currentMatchIndex, size_t totalMatches)
{
    ui->statusLabel->setText(QString("Matches: %1/%2").arg(currentMatchIndex).arg(totalMatches));
}

// --- Search Logic ---

QTextDocument::FindFlags TextMatcher::iterateFlagsFor(const SearchOptions &search) const
{
    // always iterate forward when counting/highlighting
    return search.flags & ~QTextDocument::FindBackward;
}

size_t TextMatcher::countMatches(const SearchOptions &search, bool stopAtCurrentSelection) const
{
    if (!search.isValid()) return 0;

    const auto flags = iterateFlagsFor(search);
    size_t count = 0;
    QTextCursor cursor(ui->textEdit->document());
    const QTextCursor &currentSelection = ui->textEdit->textCursor();

    while (!(cursor = ui->textEdit->document()->find(search.regex, cursor, flags)).isNull()) {
        ++count;
        if (stopAtCurrentSelection && cursor.selectionEnd() == currentSelection.selectionEnd()) {
            break;
        }
    }

    return count;
}

size_t TextMatcher::calculateTotalMatches(const SearchOptions &search) const
{
    return countMatches(search, false);
}

size_t TextMatcher::calculateCurrentMatchIndex(const SearchOptions &search) const
{
    return countMatches(search, true);
}

void TextMatcher::highlightAllMatches(const SearchOptions &search, size_t currentMatchIndex)
{
    if (!search.isValid()) {
        ui->textEdit->setExtraSelections({});
        return;
    }

    QList<QTextEdit::ExtraSelection> selections;
    QTextCursor cursor(ui->textEdit->document());
    const auto flags = iterateFlagsFor(search);

    size_t index = 0;
    while (!(cursor = ui->textEdit->document()->find(search.regex, cursor, flags)).isNull()) {
        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        ++index;
        sel.format.setBackground(index == currentMatchIndex ? CurrentHighlightColor : HighlightColor);
        selections.append(sel);
    }

    ui->textEdit->setExtraSelections(selections);
}

// --- File I/O ---

void TextMatcher::loadTextFromFile(const QString &path)
{
    QFile inputFile(path);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Error: Could not open file:" << path;
        ui->textEdit->setPlainText("Error: Could not load file.");
        return;
    }

    QTextStream in(&inputFile);
    ui->textEdit->setPlainText(in.readAll());
    inputFile.close();

    ui->lineEdit->clear();
    moveCursorToStart();
    resetSearchState();
}

void TextMatcher::handleLoadFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Text File"),
        QString(),
        tr("Text Files (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    loadTextFromFile(fileName);
}

// --- Main Find Logic ---

void TextMatcher::performFind(bool backwards)
{
    const QString searchText = ui->lineEdit->text();
    if (searchText.isEmpty()) {
        resetSearchState();
        return;
    }

    SearchOptions search(searchText, ui->wholeWordCheckbox->isChecked(), ui->caseSensitiveCheckbox->isChecked());
    if (!search.isValid()) {
        qWarning() << "Error: Invalid regex:" << search.regex.errorString();
        resetSearchState();
        return;
    }
    if (backwards) search.flags |= QTextDocument::FindBackward;

    const bool newSearch = (search.regex.pattern() != m_lastRegexPattern ||
                            search.patternOptions != m_lastPatternOptions);
    if (newSearch) {
        m_totalMatches = calculateTotalMatches(search);
        m_lastRegexPattern = search.regex.pattern();
        m_lastPatternOptions = search.patternOptions;

        backwards ? moveCursorToEnd() : moveCursorToStart();

        if (m_totalMatches == 0) {
            resetSearchState();
            return;
        }
    }

    // Try to find next match, restart if end reached
    if (!ui->textEdit->find(search.regex, search.flags)) {
        backwards ? moveCursorToEnd() : moveCursorToStart();
        ui->textEdit->find(search.regex, search.flags);
    }

    size_t currentMatchIndex = calculateCurrentMatchIndex(search);

    updateStatusLabel(currentMatchIndex, m_totalMatches);
    highlightAllMatches(search, currentMatchIndex);
}

// --- Slots ---
void TextMatcher::handleFindNext() { performFind(false); }
void TextMatcher::handleFindPrevious() { performFind(true); }
