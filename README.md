# Qt Text Matcher

A small Qt Widgets application for searching and matching text inside files.
Evolved from the Qt "Text Finder" tutorial into **TextMatcher** with enhanced search features.

## Features
- Qt Widgets UI (QLineEdit + Find button + QTextEdit)
- Search with QRegularExpression (input is escaped by default for safety)
- Whole-word matching option (uses `\b` anchors)
- Case-sensitive toggle (when off, search is case-insensitive)
- Match counting with a status label: shows current match index and total (e.g., `Matches: 3/10`)
- Proper handling for empty input and invalid regex patterns
- Loads initial content from Qt resources (`:/input.txt`)
- Simple codebase intended for learning Qt Widgets, signals/slots, and text processing

## Usage notes
- Type a keyword (or phrase) into the input and press **Find**.
- Check **Whole Word** to match only complete words (word boundaries).
- Uncheck **Case Sensitive** to perform case-insensitive matches.
- Internal logic escapes user input by default, so special regex characters (like `.` or `*`)
  are treated literally unless intentionally included in a pattern.
- If you intentionally want to provide a raw regex, you can modify `createRegexString()` to
  skip escaping; currently the app prioritizes safety for learners.

## Build (CMake + Qt)
Requires Qt 5 or Qt 6 and CMake >= 3.16.

```bash
mkdir build
cd build
cmake ..
cmake --build .
./TextMatcher