#ifndef TEXTMATCHER_H
#define TEXTMATCHER_H

#include <QWidget>
#include <QRegularExpression>

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
    void handleFindClicked();

private:
    // --- UI Pointer ---
    Ui::TextMatcher *ui;

    // --- State Variables ---
    size_t m_totalMatches = 0;
    QString m_lastRegexString;
    QRegularExpression::PatternOptions m_lastRegexOptions;

    // --- Cursor Utility ---
    void moveCursorToStart();
    void clearTextSelection();

    // --- Status Management ---
    void resetSearchState();
    void updateStatusLabel(size_t currentMatchIndex, size_t totalMatches);

    // --- Match Calculation ---
    QString createRegexString(const QString &searchString) const;
    size_t countMatches(const QRegularExpression &regex, bool stopAtCurrentSelection) const;
    size_t calculateTotalMatches(const QRegularExpression &regex) const;
    size_t calculateCurrentMatchIndex(const QRegularExpression &regex) const;

    // --- File I/O ---
    void loadTextFile();
};

#endif // TEXTMATCHER_H
