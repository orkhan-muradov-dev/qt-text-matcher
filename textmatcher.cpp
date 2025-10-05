#include "textmatcher.h"
#include "./ui_textmatcher.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextDocument>

TextMatcher::TextMatcher(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextMatcher)
{
    ui->setupUi(this);
    loadTextFile();
}

TextMatcher::~TextMatcher()
{
    delete ui;
}

// Cursor Utility

void TextMatcher::moveCursorToStart()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->textEdit->setTextCursor(cursor);
}

// Status Management

void TextMatcher::updateStatusLabel(size_t currentMatchIndex, size_t totalMatches)
{
    QString statusText = QString("Matches: %1/%2").arg(currentMatchIndex).arg(totalMatches);
    ui->statusLabel->setText(statusText);
}

// Match calculator

size_t TextMatcher::countMatches(const QString &searchString, bool stopAtCurrentSelection) const
{
    if (searchString.isEmpty()) {
        return 0;
    }

    size_t count = 0;
    QTextCursor findCursor(ui->textEdit->document());
    const QTextCursor &currentSelection = ui->textEdit->textCursor();

    while (!(findCursor = ui->textEdit->document()->find(searchString, findCursor, QTextDocument::FindWholeWords)).isNull()) {
        count++;

        if (stopAtCurrentSelection) {
            if (findCursor.hasSelection() && findCursor.selectionEnd() == currentSelection.selectionEnd()) {
                break;
            }
        }
    }

    return count;
}

size_t TextMatcher::calculateTotalMatches(const QString &searchString) const
{
    return countMatches(searchString, false);
}

size_t TextMatcher::calculateCurrentMatchIndex(const QString &searchString) const
{
    return countMatches(searchString, true);
}

// FILE I/O

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

    updateStatusLabel(0, 0);
}

// FIND LOGIC

void TextMatcher::on_findButton_clicked()
{
    QString searchString = ui->lineEdit->text();

    if (searchString != m_lastSearchString) {
        m_totalMatches = calculateTotalMatches(searchString);
        m_lastSearchString = searchString;
        moveCursorToStart();

        if (m_totalMatches == 0) {
            updateStatusLabel(0, 0);
            return;
        }
    }

    if (!ui->textEdit->find(searchString, QTextDocument::FindWholeWords)) {
        moveCursorToStart();
        ui->textEdit->find(searchString, QTextDocument::FindWholeWords);
    }

    size_t currentMatchIndex = calculateCurrentMatchIndex(searchString);

    updateStatusLabel(currentMatchIndex, m_totalMatches);
}
