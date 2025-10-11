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
    constexpr auto DEFAULT_INPUT_FILE = ":/input.txt";
    constexpr auto FILE_DIALOG_FILTER = "Text Files (*.txt);;All Files (*)";

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

    setupConnections();
    setupKeyboardShortcuts();
    loadTextFromFile(DEFAULT_INPUT_FILE);
}

TextMatcher::~TextMatcher() { delete ui; }

// --- Initialization Methods ---

void TextMatcher::setupConnections()
{
    // Button connections
    connect(ui->findNextButton, &QPushButton::clicked, this, &TextMatcher::handleFindNext);
    connect(ui->findPrevButton, &QPushButton::clicked, this, &TextMatcher::handleFindPrevious);
    connect(ui->loadFileButton, &QPushButton::clicked, this, &TextMatcher::handleLoadFile);

    // Enter key in search field triggers find next
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, [this]() {
        ui->findNextButton->animateClick();
    });
}

void TextMatcher::setupKeyboardShortcuts()
{
    // Down arrow - find next
    auto *shortcutDown = new QShortcut(QKeySequence(Qt::Key_Down), this);
    connect(shortcutDown, &QShortcut::activated, this, [this]() {
        ui->findNextButton->animateClick();
    });

    // Up arrow - find previous
    auto *shortcutUp = new QShortcut(QKeySequence(Qt::Key_Up), this);
    connect(shortcutUp, &QShortcut::activated, this, [this]() {
        ui->findPrevButton->animateClick();
    });

    // Ctrl+O - Open File
    auto *shortcutOpen = new QShortcut(QKeySequence(QKeySequence::Open), this);
    connect(shortcutOpen, &QShortcut::activated, ui->loadFileButton, &QPushButton::animateClick);

    // Ctrl+L - Clear Line Edit
    auto *shortcutClear = new QShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_L), this);
    connect(shortcutClear, &QShortcut::activated, this, &TextMatcher::handleClearLineEdit);

    // Ctrl+Q - Quit application
    auto *shortcutQuit = new QShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_Q), this);
    connect(shortcutQuit, &QShortcut::activated, qApp, &QApplication::quit);
}

// --- Cursor Management ---

void TextMatcher::moveCursorToStart()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->textEdit->setTextCursor(cursor);
}

void TextMatcher::moveCursorToEnd()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}

void TextMatcher::clearTextSelection()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.clearSelection();
    ui->textEdit->setTextCursor(cursor);
}

// --- Search State ---

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
    ui->statusLabel->setText(tr("Matches: %1/%2").arg(currentMatchIndex).arg(totalMatches));
}

bool TextMatcher::isNewSearch(const SearchOptions &search) const
{
    return search.regex.pattern() != m_lastRegexPattern ||
           search.patternOptions != m_lastPatternOptions;
}

// --- Search Logic ---

QTextDocument::FindFlags TextMatcher::iterateFlagsFor(const SearchOptions &search) const
{
    // Always iterate forward when counting/highlighting (mask out FindBackward)
    return search.flags & ~QTextDocument::FindBackward;
}

size_t TextMatcher::countMatches(const SearchOptions &search, bool stopAtCurrentSelection) const
{
    if (!search.isValid()) return 0;

    const auto flags = iterateFlagsFor(search);
    QTextCursor cursor(ui->textEdit->document());
    const QTextCursor &currentSelection = ui->textEdit->textCursor();

    size_t count = 0;
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
        ++index;

        QTextEdit::ExtraSelection selection;
        selection.cursor = cursor;

        selection.format.setBackground(index == currentMatchIndex ? CurrentHighlightColor : HighlightColor);
        selections.append(selection);
    }

    ui->textEdit->setExtraSelections(selections);
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

    // Set search direction
    if (backwards) search.flags |= QTextDocument::FindBackward;

    if (isNewSearch(search)) {
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

    // Update UI
    size_t currentMatchIndex = calculateCurrentMatchIndex(search);
    updateStatusLabel(currentMatchIndex, m_totalMatches);
    highlightAllMatches(search, currentMatchIndex);
}

// --- File I/O ---

void TextMatcher::loadTextFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Error: Could not open file:" << path;
        ui->textEdit->setPlainText(tr("Error: Could not load file."));
        return;
    }

    QTextStream stream(&file);
    ui->textEdit->setPlainText(stream.readAll());
    file.close();

    QFileInfo fi(path);
    m_lastFolder = fi.absolutePath();

    // Reset UI state
    ui->lineEdit->clear();
    ui->wholeWordCheckbox->setChecked(false);
    ui->caseSensitiveCheckbox->setChecked(false);
    moveCursorToStart();
    resetSearchState();
}

// --- Slots ---

void TextMatcher::handleFindNext() { performFind(false); }

void TextMatcher::handleFindPrevious() { performFind(true); }

void TextMatcher::handleLoadFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Text File"),
        m_lastFolder.isEmpty() ? QString() : m_lastFolder,
        tr(FILE_DIALOG_FILTER)
    );

    if (!fileName.isEmpty())
        loadTextFromFile(fileName);
}

void TextMatcher::handleClearLineEdit()
{
    ui->lineEdit->clear();
    resetSearchState();
    ui->lineEdit->setFocus();
}
