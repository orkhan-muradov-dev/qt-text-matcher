#include "textfinder.h"
#include "./ui_textfinder.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

TextFinder::TextFinder(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextFinder)
{
    ui->setupUi(this);
    loadTextFile();
}

TextFinder::~TextFinder()
{
    delete ui;
}

void TextFinder::moveCursorStart()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->textEdit->setTextCursor(cursor);
}

void TextFinder::on_findButton_clicked()
{
    QString searchString = ui->lineEdit->text();

    if (!ui->textEdit->find(searchString, QTextDocument::FindWholeWords)) {
        moveCursorStart();
        ui->textEdit->find(searchString, QTextDocument::FindWholeWords);
    }
}

void TextFinder::loadTextFile()
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
    moveCursorStart();
 }
