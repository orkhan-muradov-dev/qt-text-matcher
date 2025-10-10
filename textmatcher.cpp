#include "textmatcher.h"
#include "./ui_textmatcher.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

TextMatcher::TextMatcher(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextMatcher)
{
    ui->setupUi(this);
    connect(ui->findButton, &QPushButton::clicked, this, &TextMatcher::handleFindClicked);
    loadTextFile();
}

TextMatcher::~TextMatcher()
{
    delete ui;
}

// --- Cursor Utility ---

void TextMatcher::moveCursorToStart()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->textEdit->setTextCursor(cursor);
}

void TextMatcher::clearTextSelection()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.clearSelection();
    ui->textEdit->setTextCursor(cursor);
}

// --- Status Management ---

void TextMatcher::resetSearchState()
{
    clearTextSelection();
    m_totalMatches = 0;
    m_lastRegexPattern.clear();
    m_lastPatternOptions = QRegularExpression::NoPatternOption;
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

    // Find flags setup
    opts.flags = (ui->caseSensitiveCheckbox->isChecked()
                      ? QTextDocument::FindCaseSensitively
                      : QTextDocument::FindFlags());

    return opts;
}

size_t TextMatcher::countMatches(const SearchOptions &search, bool stopAtCurrentSelection) const
{
    if (!search.regex.isValid() || search.regex.pattern().isEmpty())
        return 0;

    size_t count = 0;
    QTextCursor cursor(ui->textEdit->document());
    const QTextCursor &currentSelection = ui->textEdit->textCursor();

    while (!(cursor = ui->textEdit->document()->find(search.regex, cursor, search.flags)).isNull()) {
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

void TextMatcher::handleFindClicked()
{
    const QString searchText = ui->lineEdit->text();
    if (searchText.isEmpty()) {
        resetSearchState();
        return;
    }

    const SearchOptions search = buildSearchOptions(searchText);
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
        moveCursorToStart();

        if (m_totalMatches == 0) {
            resetSearchState();
            return;
        }
    }

    // Try to find next match, restart if end reached
    if (!ui->textEdit->find(search.regex, search.flags)) {
        moveCursorToStart();
        ui->textEdit->find(search.regex, search.flags);
    }

    const size_t currentMatchIndex = calculateCurrentMatchIndex(search);
    updateStatusLabel(currentMatchIndex, m_totalMatches);
}
