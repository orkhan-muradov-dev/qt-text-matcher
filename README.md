# Qt Text Matcher

A Qt Widgets application for searching and matching text inside files.
Evolved from the Qt "Text Finder" tutorial into **TextMatcher** with progressive enhancements.

## Features
- Qt Widgets UI (QLineEdit + Find Prev/Next + Load button + QTextEdit)
- Load text files from disk via **Load** button (uses native file dialog).
- Search uses `QRegularExpression` with input escaping by default (safe mode).
- Whole-word matching option (wraps escaped input with `\b` anchors).
- Case-sensitive toggle — applied consistently via both regex options and `QTextDocument::FindFlags`.
- Centralized `SearchOptions` abstraction (pattern + find flags) to ensure consistent behavior.
- Match counting with a status label: shows current match index and total (e.g., `Matches: 3/10`).
- Highlights all matches in the document using `QTextEdit::ExtraSelection` and visually emphasizes the current match.
- Navigation controls:
  - **Prev / Next** UI buttons
  - Press **Enter** to trigger Find Next
  - Keyboard shortcuts: **Up** = previous, **Down** = next
- UI polish:
  - Clear button in the search input for quick clearing.
  - Window icon set from standard UI theme.
  - Highlight colors extracted to constants for easy tuning.
- Proper handling for empty input and invalid regex patterns.
- Loads default content from Qt resources (`:/input.txt`) on startup.

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
- Consider adding a file-recent menu or drag-and-drop file loading in a future update.

## License
**MIT** — see [LICENSE]((./LICENSE.txt)) file.