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
    void handleLoadFile();

private:
    // --- UI Pointer ---
    Ui::TextMatcher *ui;

    // --- Search Options Struct ---
    struct SearchOptions {
        QRegularExpression regex;
        QTextDocument::FindFlags flags;
        QRegularExpression::PatternOptions patternOptions;

        SearchOptions(const QString &text, bool wholeWord, bool caseSensitive);

        bool isValid() const;
    };

    // --- State Variables ---
    size_t m_totalMatches = 0;
    QString m_lastRegexPattern;
    QRegularExpression::PatternOptions m_lastPatternOptions;
    QString m_lastFolder;

    // --- Initialization ---
    void setupConnections();
    void setupKeyboardShortcuts();

    // --- Cursor Management ---
    void moveCursorToStart();
    void moveCursorToEnd();
    void clearTextSelection();

    // --- Search State ---
    void resetSearchState();
    void updateStatusLabel(size_t currentMatchIndex, size_t totalMatches);
    bool isNewSearch(const SearchOptions &search) const;

    // --- Search Logic ---
    size_t countMatches(const SearchOptions &search, bool stopAtCurrentSelection) const;
    size_t calculateTotalMatches(const SearchOptions &search) const;
    size_t calculateCurrentMatchIndex(const SearchOptions &search) const;
    QTextDocument::FindFlags iterateFlagsFor(const SearchOptions &search) const; // for forward iteration (mask out FindBackward)

    void highlightAllMatches(const SearchOptions &search, size_t currentMatchIndex);
    void performFind(bool backwards);

    // --- File I/O ---
    void loadTextFromFile(const QString &path);
};

#endif // TEXTMATCHER_H
