#ifndef TEXTMATCHER_H
#define TEXTMATCHER_H

#include <QWidget>
#include <QTextDocument>
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
    void handleFindNext();
    void handleFindPrevious();

private:
    // --- UI Pointer ---
    Ui::TextMatcher *ui;

    // --- Search Options Struct ---
    struct SearchOptions {
        QRegularExpression regex;
        QTextDocument::FindFlags flags;
        QRegularExpression::PatternOptions patternOptions;
    };

    // --- State Variables ---
    size_t m_totalMatches = 0;
    QString m_lastRegexPattern;
    QRegularExpression::PatternOptions m_lastPatternOptions;

    // --- Cursor Utility ---
    void moveCursorToStart();
    void moveCursorToEnd();
    void clearTextSelection();

    // --- Status Management ---
    void resetSearchState();
    void updateStatusLabel(size_t currentMatchIndex, size_t totalMatches);

    // --- Search Logic ---
    QString createRegexPattern(const QString &searchText) const;
    SearchOptions buildSearchOptions(const QString &searchText) const;

    size_t countMatches(const SearchOptions &search, bool stopAtCurrentSelection) const;
    size_t calculateTotalMatches(const SearchOptions &search) const;
    size_t calculateCurrentMatchIndex(const SearchOptions &search) const;

    // produce flags used for forward iteration (mask out FindBackward)
    QTextDocument::FindFlags iterateFlagsFor(const SearchOptions &search) const;

    void highlightAllMatches(const SearchOptions &search, size_t currentMatchIndex);

    void performFind(bool backwards);

    // --- File I/O ---
    void loadTextFile();
};

#endif // TEXTMATCHER_H
