# Qt Text Matcher

A small Qt Widgets application for searching and matching text inside files.
Evolved from the Qt "Text Finder" tutorial into **TextMatcher** with enhanced UX.

## Features
- Qt Widgets UI (QLineEdit + Find button + QTextEdit)
- Whole-word search with wrap-around behavior
- Match counting with a status label: shows current match index and total (e.g., `Matches: 3/10`)
- Loads initial content from Qt resources (`:/input.txt`)
- Simple, easy-to-understand codebase for learning Qt Widgets and I/O

## Build (CMake + Qt)
Requires Qt 5 or Qt 6 and CMake >= 3.16.

```bash
mkdir build
cd build
cmake ..
cmake --build .
./TextMatcher
