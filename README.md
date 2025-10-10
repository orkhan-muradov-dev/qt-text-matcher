# Qt Text Matcher

A small Qt Widgets application for searching and matching text inside files.
Evolved from the Qt "Text Finder" tutorial into **TextMatcher** with progressive enhancements.

## Features
- Qt Widgets UI (QLineEdit + Find button + QTextEdit)
- Search uses `QRegularExpression` with input escaping by default (safe mode)
- Whole-word matching option (wraps escaped input with `\b` anchors)
- Case-sensitive toggle — applied consistently via both regex options and
  `QTextDocument::FindFlags` so highlighted matches and counting align
- Centralized `SearchOptions` abstraction (pattern + find flags) to ensure
  consistent behavior across searching and counting
- Match counting with a status label: shows current match index and total (e.g., `Matches: 3/10`)
- Proper handling for empty input and invalid regex patterns
- Loads initial content from Qt resources (`:/input.txt`)
- Simple, easy-to-understand codebase for learning Qt Widgets, signals/slots, and text processing

## Build (CMake + Qt)
Requires Qt 5 or Qt 6 and CMake >= 3.16.

```bash
mkdir build
cd build
cmake ..
cmake --build .
./TextMatcher
```

## Contributing / Notes
- By default the app escapes user input to avoid accidental regex injection and unexpected matches.
- The SearchOptions abstraction makes it straightforward to later add features (e.g., "raw regex" advanced mode, whole-directory search, or thread-pool scanning).

## License
**MIT** — see [LICENSE]((./LICENSE.txt)) file.