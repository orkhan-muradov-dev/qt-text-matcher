#include "textmatcher.h"
#include "./ui_textmatcher.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextDocument>
#include <QRegularExpression>

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
    m_lastRegexString.clear();
    m_lastRegexOptions = QRegularExpression::NoPatternOption;
    updateStatusLabel(0, 0);
}

void TextMatcher::updateStatusLabel(size_t currentMatchIndex, size_t totalMatches)
{
    QString statusText = QString("Matches: %1/%2").arg(currentMatchIndex).arg(totalMatches);
    ui->statusLabel->setText(statusText);
}

// --- Match Calculation ---

QString TextMatcher::createRegexString(const QString &searchString) const
{
    QString escapedString = QRegularExpression::escape(searchString);

    if (ui->wholeWordCheckbox->isChecked()) {
        return QStringLiteral("\\b") + escapedString + QStringLiteral("\\b");
    } else {
        return escapedString;
    }
}

size_t TextMatcher::countMatches(const QRegularExpression &regex, bool stopAtCurrentSelection) const
{
    if (regex.pattern().isEmpty()) {
        return 0;
    }

    size_t count = 0;
    QTextCursor findCursor(ui->textEdit->document());
    const QTextCursor &currentSelection = ui->textEdit->textCursor();

    while (!(findCursor = ui->textEdit->document()->find(regex, findCursor)).isNull()) {
        count++;

        if (stopAtCurrentSelection) {
            if (findCursor.selectionEnd() == currentSelection.selectionEnd()) {
                break;
            }
        }
    }

    return count;
}

size_t TextMatcher::calculateTotalMatches(const QRegularExpression &regex) const
{
    return countMatches(regex, false);
}

size_t TextMatcher::calculateCurrentMatchIndex(const QRegularExpression &regex) const
{
    return countMatches(regex, true);
}

// --- FILE I/O ---

void TextMatcher::loadTextFile()
{
    QFile inputFile(":/input.txt");

    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Error: Could not open resource file ':/input.txt'.";
        ui->textEdit->setPlainText("Error: Could not load initial text.");
        return;
    }

    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();

    ui->textEdit->setPlainText(line);
    moveCursorToStart();

    resetSearchState();
}

// --- FIND LOGIC ---

void TextMatcher::handleFindClicked()
{
    QString searchString = ui->lineEdit->text();

    // Stage 1: Input Check and Early Exit
    if (searchString.isEmpty()) {
        resetSearchState();
        return;
    }

    QString regexString = createRegexString(searchString);
    QRegularExpression::PatternOptions currentOptions = QRegularExpression::NoPatternOption;
    if (!ui->caseSensitiveCheckbox->isChecked()) {
        currentOptions |= QRegularExpression::CaseInsensitiveOption;
    }
    QRegularExpression regex(regexString, currentOptions);

    if (!regex.isValid()) {
        qWarning() << "Error: Invalid regular expression generated:" << regex.errorString();
        resetSearchState();
        return;
    }

    // Stage 2: State Check (Is this a new search?)
    if (regexString != m_lastRegexString || currentOptions != m_lastRegexOptions) {
        m_totalMatches = calculateTotalMatches(regex);
        m_lastRegexString = regexString;
        m_lastRegexOptions = currentOptions;
        moveCursorToStart();

        if (m_totalMatches == 0) {
            resetSearchState();
            return;
        }
    }

    // Stage 3: Execute Find and Update Status
    if (!ui->textEdit->find(regex)) {
        moveCursorToStart();
        ui->textEdit->find(regex);
    }

    size_t currentMatchIndex = calculateCurrentMatchIndex(regex);
    updateStatusLabel(currentMatchIndex, m_totalMatches);
}
