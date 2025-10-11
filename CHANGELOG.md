# Changelog

All notable changes to **TextMatcher** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Search history dropdown
- Replace functionality (find and replace)
- Recent files list
- Preferences/settings dialog
- Automated testing framework with Qt Test

---

## [0.8.3] - 2025-10-11

### Fixed
- Replace demo GIF (media/demo-navigation.gif) to remove black edges for improved visuals.

---

## [0.8.2] - 2025-10-11

### Added
- Keyboard toggles:
  - Alt+S -> toggle Case Sensitive
  - Alt+W -> toggle Whole Word
- Demo GIF added to README for an animated preview.

### Fixed
- Clear-on-empty bug: search now resets when the search field is cleared (clear button or manual deletion).
- Reordered resetSearchState() logic to avoid stale selection and indexing after clearing the search.

### Changed
- Replaced explicit clear slot with inline lambda handlers for a simpler and more consistent action path.
- Minor qrc path cleanup.

---

## [0.8.1] - 2025-10-11

### Fixed
- Adjust screenshots width in README for better layout on GitHub

---

## [0.8.0] - 2025-10-11

### Added
- Global shortcuts:
  - **Ctrl+O** — Open file (animates Load button)
  - **Ctrl+L** — Clear search input and reset search state
  - **Ctrl+Q** — Quit application
- `handleClearLineEdit()` — clear search field, reset highlights and focus input
- Project layout reorganization: sources moved to `src/`, assets to `media/`
- README and CHANGELOG updated to reflect new layout and features
- CONTRIBUTING.md

### Changed
- File loading now clears search options and resets UI state (whole-word & case-sensitive defaults)

### Notes
- This is a UX + project-organization release (no breaking API changes).

---

## [0.7.0] - 2025-10-10

### Added
- **Smart Directory Memory**: Application now remembers the last opened folder, making subsequent file loads more convenient
- **Cleaner Architecture**: Extracted `setupConnections()` and `setupKeyboardShortcuts()` initialization methods for better code organization and maintainability
- **Unified Action Handling**: All search triggers (Enter key, button clicks, keyboard shortcuts) now use a single codepath via `animateClick()` for consistent behavior
- **Visual Feedback**: Button animations now work consistently across all interaction methods

### Changed
- **Enhanced File Loading**: `loadTextFromFile()` now performs comprehensive cleanup:
  - Stores the directory path of the loaded file for future file dialogs
  - Automatically clears the search input field
  - Resets search state and highlighting
  - Moves cursor to document start for consistent UX
- **Code Organization**: Separated initialization logic from constructor for improved readability
- **Improved Maintainability**: Reduced code duplication in action handling

### Technical Details
- Introduced `m_lastFolder` member variable to persist directory state
- Refactored constructor to delegate setup to specialized methods
- Connected all navigation triggers to a single execution path

### Notes
- This release focuses on quality-of-life improvements and code maintainability
- No breaking changes to existing functionality
- File format and API remain backward compatible

---

## [0.6.0] - 2025-10-10

### Added
- **File Loading Capability**: Users can now load text files from disk via the "Load File" button or menu
- **Native File Dialog**: Integrated QFileDialog for system-native file selection experience
- **SearchOptions Struct**: Introduced dedicated `SearchOptions` class for cleaner search parameter management
  - Encapsulates `QRegularExpression`, find flags, and pattern options
  - Includes validation via `isValid()` method
- **UI Polish**: 
  - Added clear button to search input (QLineEdit clearButtonEnabled property)
  - Set application window icon using standard Qt icons
- **Error Handling**: Graceful handling of file read errors with user feedback

### Changed
- Refactored search logic to use `SearchOptions` abstraction instead of scattered parameters
- Improved code modularity and type safety with structured search parameters

### Technical Details
- `SearchOptions` constructor handles regex compilation and flag configuration
- Window icon uses `QStyle::SP_FileDialogContentsView` for consistent system appearance
- File reading uses `QTextStream` for proper text encoding handling

### Fixed
- Ensured proper cleanup when loading new files
- Improved error messages for file operation failures

---

## [0.5.0] - 2025-10-10

### Added
- **Bidirectional Navigation**: 
  - "Find Previous" button for backward search
  - Automatic wraparound at document start/end for seamless navigation
- **Enhanced Keyboard Support**:
  - **Up Arrow (↑)**: Navigate to previous match
  - **Down Arrow (↓)**: Navigate to next match  
  - **Enter Key**: Trigger "Find Next" directly from search field
- **Smart Button Interaction**: Connected Enter key to button animation for consistent feedback

### Changed
- **Consistent Iteration Logic**: Introduced `iterateFlagsFor()` helper method to ensure counting and highlighting always iterate forward (masking out `FindBackward` flag)
- **Improved Navigation UX**: Cursor position management refined for bidirectional search

### Technical Details
- `performFind()` now accepts `bool backwards` parameter
- Forward/backward search share core logic with directional flag modification
- QShortcut bindings for arrow keys enable keyboard-first workflow

### Fixed
- Resolved edge case where backward navigation could miss matches
- Fixed wraparound behavior at document boundaries

---

## [0.4.0] - 2025-10-10

### Added
- **Full Match Highlighting**: All search results now highlighted simultaneously in the document
- **Dual-Color System**: 
  - Current match: Distinct highlight color (orange-brown: RGB 100, 52, 17)
  - Other matches: Secondary highlight color (yellow-brown: RGB 100, 92, 17)
- **Visual Navigation**: Current match automatically emphasized as user navigates
- **Enter Key Trigger**: Pressing Enter in search field initiates or continues search

### Changed
- Implemented `QTextEdit::ExtraSelection` for performant multi-match highlighting
- `highlightAllMatches()` method creates visual overlay for all matches
- Match counter calculation now stops at current selection for efficiency

### Technical Details
- Uses `QTextEdit::setExtraSelections()` for non-invasive highlighting
- Highlight colors defined as module-level constants for easy customization
- Efficient iteration: counts only up to current position when determining match index

### Performance
- Optimized match counting with early exit (`stopAtCurrentSelection` parameter)
- Single-pass highlighting algorithm

---

## [0.3.0] - 2025-10-10

### Added
- **SearchOptions Abstraction**: Introduced structured search parameter management
  - Bundles `QRegularExpression`, `FindFlags`, and `PatternOptions`
  - Validation method ensures search integrity before execution

### Changed
- **Unified Search Logic**: Refactored all search operations to use `SearchOptions` struct
- **Improved Code Clarity**: Reduced parameter passing, enhanced type safety

### Technical Details
- `SearchOptions` struct encapsulates:
  - `QRegularExpression regex`: Compiled search pattern
  - `QTextDocument::FindFlags flags`: Search direction and options
  - `QRegularExpression::PatternOptions patternOptions`: Case sensitivity tracking
- Constructor handles regex compilation with proper escaping
- `isValid()` method prevents invalid pattern execution

### Benefits
- Easier to extend with new search parameters
- Reduced function parameter count
- Centralized validation logic

---

## [0.2.1] - 2025-10-05

### Added
- **Comprehensive Documentation**:
  - Created this CHANGELOG.md following Keep a Changelog format
  - Updated README.md with improved formatting and content
  - Added contributing guidelines

### Changed
- Minor documentation improvements for clarity

### Notes
- Documentation-only release, no functional changes
- Establishes version history tracking

---

## [0.2.0] - 2025-10-03

### Added
- **Regex-Based Search Engine**: Complete rewrite using `QRegularExpression` (PCRE2 backend)
  - Supports full regular expression patterns internally
  - Automatic escaping of user input for literal matching
- **Advanced Search Options**:
  - **Whole Word Matching**: Toggle via checkbox (`\b` word boundaries in regex)
  - **Case Sensitivity**: Toggle via checkbox (CaseInsensitiveOption flag)
- **Match Counting System**: 
  - Real-time total match count calculation
  - Current match position tracking (e.g., "Match 3 of 15")
  - Status label displays "Matches: X/Y" format
- **Smart Search State Management**:
  - Caches total match count to avoid redundant calculations
  - Tracks last search pattern to detect new searches
  - Automatic reset when search parameters change

### Changed
- **Search Algorithm**: Migrated from simple `QString::contains()` to regex-based matching
- **UI Updates**: Added status label for match counter display
- **State Tracking**: Introduced member variables:
  - `m_totalMatches`: Cached count of total matches
  - `m_lastRegexPattern`: Previous search pattern for comparison
  - `m_lastPatternOptions`: Previous pattern options for change detection

### Technical Details
- `countMatches()`: Efficient iterative counting using `QTextDocument::find()`
- `calculateCurrentMatchIndex()`: Determines position by counting from start
- `isNewSearch()`: Detects pattern or option changes to trigger re-count
- Word boundary regex: `\b` + escaped_text + `\b`

### Performance
- Match counting optimized with early exit for current position
- Single-pass algorithms for both counting and navigation

---

## [0.1.0] - 2025-09-28

### Added
- **Initial Release**: Core text search application based on Qt tutorial
- **Basic Search Functionality**: Simple substring matching using `QString::contains()`
- **Whole Word Search**: Checkbox for complete word matching
- **User Interface**:
  - Search input field (QLineEdit)
  - "Find" button for triggering search
  - Text display area (QTextEdit)
  - Whole word checkbox
- **Qt Designer Integration**: UI created with `.ui` file
- **CMake Build System**: Modern CMake configuration for cross-platform builds

### Technical Details
- Qt6 Widgets application
- Basic QTextCursor navigation
- Simple forward-only search
- No regex support (literal string matching)

### Known Limitations
- No backward search
- No match counter
- No highlighting of multiple matches
- No case sensitivity toggle
- Search doesn't wrap around document

---

## Version History Summary

| Version |    Date    |                 Key Features                 |
|---------|------------|----------------------------------------------|
|  0.7.0  | 2025-10-10 | Smart directory memory, cleaner architecture |
|  0.6.0  | 2025-10-10 | File loading, SearchOptions struct           |
|  0.5.0  | 2025-10-10 | Bidirectional navigation, keyboard shortcuts |
|  0.4.0  | 2025-10-10 | Multi-match highlighting, dual colors        |
|  0.3.0  | 2025-10-10 | SearchOptions abstraction                    |
|  0.2.1  | 2025-10-05 | Documentation improvements                   |
|  0.2.0  | 2025-10-03 | Regex engine, match counter, search options  |
|  0.1.0  | 2025-09-28 | Initial release                              |

---

## Upgrade Guide

### From 0.6.x to 0.7.0
- **No Breaking Changes**: Direct drop-in replacement
- **New Features**: Automatic directory memory (no configuration needed)
- **Note**: Previously opened file paths are not migrated (fresh state)

### From 0.5.x to 0.6.0
- **No Breaking Changes**: Seamless upgrade
- **New Capability**: Use "Load File" button to import text files
- **Improvement**: SearchOptions struct improves internal code quality

### From 0.4.x to 0.5.0
- **No Breaking Changes**: Fully backward compatible
- **New Controls**: Use ↑/↓ arrow keys for navigation
- **Enhancement**: Enter key now works in search field

### From 0.3.x to 0.4.0
- **No Breaking Changes**: API compatible
- **Visual Change**: All matches now highlighted (may look different)
- **Note**: Two-color highlighting distinguishes current match

### From 0.2.x to 0.3.0
- **No Breaking Changes**: Internal refactoring only
- **Note**: SearchOptions improves code organization (not visible to users)

### From 0.1.x to 0.2.0
- **Behavioral Change**: Search is now case-insensitive by default
  - **Migration**: Enable "Case Sensitive" checkbox to restore old behavior
- **New Features**: Match counter shows position in results
- **Enhancement**: Regex support (though special chars are escaped)

---

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

When submitting changes:
1. **Update this CHANGELOG** under `[Unreleased]` section
2. **Follow format**: Added, Changed, Deprecated, Removed, Fixed, Security
3. **Include context**: Why the change was made, not just what changed
4. **Reference issues**: Link to related GitHub issues (e.g., `Fixes #42`)

---

## Support

- **Bug Reports**: [GitHub Issues](https://github.com/orkhan-muradov-dev/qt-text-matcher/issues)
- **Discussions**: [GitHub Discussions](https://github.com/orkhan-muradov-dev/qt-text-matcher/discussions)
- **Email**: orkhan.muradov.business@gmail.com

---

<div align="center">

**[View All Releases](https://github.com/orkhan-muradov-dev/qt-text-matcher/releases)** | 
**[Back to README](README.md)**

</div>