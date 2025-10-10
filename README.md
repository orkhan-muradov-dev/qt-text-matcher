# Qt Text Matcher

A Qt Widgets application for searching and matching text inside files.
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
- **Highlights all matches** in the document using `QTextEdit::ExtraSelection`
  and visually emphasizes the current match for easier navigation
- Press **Enter** in the search field to trigger a search (convenience UX)
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
- To allow raw regex input later, add an “Advanced: raw regex” option that bypasses escaping in `createRegexPattern()`.
- `highlightAllMatches()` uses `QTextEdit::ExtraSelection`; colors are basic and can be tuned for accessibility.

## License
**MIT** — see [LICENSE]((./LICENSE.txt)) file.