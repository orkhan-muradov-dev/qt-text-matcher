#include "textmatcher.h"
#include "./ui_textmatcher.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QShortcut>

TextMatcher::TextMatcher(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextMatcher)
{
    ui->setupUi(this);

    // Buttons
    connect(ui->findNextButton, &QPushButton::clicked, this, &TextMatcher::handleFindNext);
    connect(ui->findPrevButton, &QPushButton::clicked, this, &TextMatcher::handleFindPrevious);

    // Enter in the search field => Find Next
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &TextMatcher::handleFindNext);

    // Keyboard shortcuts: Up/Down = previous/next
    new QShortcut(QKeySequence(Qt::Key_Up), this, SLOT(handleFindPrevious()));
    new QShortcut(QKeySequence(Qt::Key_Down), this, SLOT(handleFindNext()));

    loadTextFile();
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
    ui->statusLabel->setText(QString("Matches: %1/%2")
                                 .arg(currentMatchIndex)
                                 .arg(totalMatches));
}

// --- Search Logic ---

QString TextMatcher::createRegexPattern(const QString &searchText) const
{
    const QString escaped = QRegularExpression::escape(searchText);
    return ui->wholeWordCheckbox->isChecked()
               ? QStringLiteral("\\b") + escaped + QStringLiteral("\\b")
               : escaped;
}

TextMatcher::SearchOptions TextMatcher::buildSearchOptions(const QString &searchText) const
{
    SearchOptions opts;

    // Regex setup
    const QString pattern = createRegexPattern(searchText);
    opts.patternOptions = (ui->caseSensitiveCheckbox->isChecked()
                               ? QRegularExpression::NoPatternOption
                               : QRegularExpression::CaseInsensitiveOption);
    opts.regex = QRegularExpression(pattern, opts.patternOptions);

    // Find flags setup (FindBackward is added by performFind when requested)
    opts.flags = (ui->caseSensitiveCheckbox->isChecked()
                      ? QTextDocument::FindCaseSensitively
                      : QTextDocument::FindFlags());

    return opts;
}

QTextDocument::FindFlags TextMatcher::iterateFlagsFor(const SearchOptions &search) const
{
    // when building lists (counting/highlighting) always iterate forward
    return search.flags & ~QTextDocument::FindBackward;
}

size_t TextMatcher::countMatches(const SearchOptions &search, bool stopAtCurrentSelection) const
{
    if (!search.regex.isValid() || search.regex.pattern().isEmpty()) return 0;

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
    if (!search.regex.isValid() || search.regex.pattern().isEmpty()) {
        ui->textEdit->setExtraSelections({});
        return;
    }

    const auto flags = iterateFlagsFor(search);

    QList<QTextEdit::ExtraSelection> selections;
    QTextCursor cursor(ui->textEdit->document());
    size_t index = 0;

    while (!(cursor = ui->textEdit->document()->find(search.regex, cursor, flags)).isNull()) {
        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;

        ++index;
        sel.format.setBackground(QColor(100, (index == currentMatchIndex ? 52 : 92), 17));

        selections.append(sel);
    }

    ui->textEdit->setExtraSelections(selections);
}

// --- File I/O ---

void TextMatcher::loadTextFile()
{
    QFile inputFile(":/input.txt");
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Error: Could not open resource file ':/input.txt'.";
        ui->textEdit->setPlainText("Error: Could not load initial text.");
        return;
    }

    QTextStream in(&inputFile);
    ui->textEdit->setPlainText(in.readAll());
    inputFile.close();

    moveCursorToStart();
    resetSearchState();
}

// --- Main Find Logic ---

void TextMatcher::performFind(bool backwards)
{
    const QString searchText = ui->lineEdit->text();
    if (searchText.isEmpty()) {
        resetSearchState();
        return;
    }

    SearchOptions search = buildSearchOptions(searchText);
    if (backwards) search.flags |= QTextDocument::FindBackward;

    if (!search.regex.isValid()) {
        qWarning() << "Error: Invalid regex:" << search.regex.errorString();
        resetSearchState();
        return;
    }

    const bool newSearch = (search.regex.pattern() != m_lastRegexPattern ||
                            search.patternOptions != m_lastPatternOptions);
    if (newSearch) {
        m_totalMatches = calculateTotalMatches(search);
        m_lastRegexPattern = search.regex.pattern();
        m_lastPatternOptions = search.patternOptions;

        if (backwards) moveCursorToEnd();
        else moveCursorToStart();

        if (m_totalMatches == 0) {
            resetSearchState();
            return;
        }
    }

    // Try to find next match, restart if end reached
    if (!ui->textEdit->find(search.regex, search.flags)) {
        if (backwards) moveCursorToEnd();
        else moveCursorToStart();
        ui->textEdit->find(search.regex, search.flags);
    }

    size_t currentMatchIndex = calculateCurrentMatchIndex(search);

    updateStatusLabel(currentMatchIndex, m_totalMatches);
    highlightAllMatches(search, currentMatchIndex);
}

void TextMatcher::handleFindNext()
{
    performFind(false);
}

void TextMatcher::handleFindPrevious()
{
    performFind(true);
}
