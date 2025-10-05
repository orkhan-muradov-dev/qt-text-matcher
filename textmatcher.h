#ifndef TEXTMATCHER_H
#define TEXTMATCHER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TextMatcher; }
QT_END_NAMESPACE

class TextMatcher : public QWidget
{
    Q_OBJECT

public:
    TextMatcher(QWidget *parent = nullptr);
    ~TextMatcher();

private slots:
    void on_findButton_clicked();

private:
    Ui::TextMatcher *ui;

    // State Variables
    size_t m_totalMatches = 0;
    QString m_lastSearchString;

    // Cursor Utility
    void moveCursorToStart();

    // Status Management
    void updateStatusLabel(size_t currentMatchIndex, size_t totalMatches);

    // Match calculator
    size_t countMatches(const QString &searchString, bool stopAtCurrentSelection) const;
    size_t calculateTotalMatches(const QString &searchString) const;
    size_t calculateCurrentMatchIndex(const QString &searchString) const;

    // File I/O
    void loadTextFile();
};
#endif // TEXTMATCHER_H
