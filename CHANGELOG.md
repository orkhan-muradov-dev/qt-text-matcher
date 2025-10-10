# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog** and this project follows Semantic Versioning.

## Unreleased
- (Work in progress)

---

## [0.7.0] - 2025-10-10
### Added
- Remember last opened folder (`m_lastFolder`) so file dialogs start in the last-used directory.
- Extracted initialization methods `setupConnections()` and `setupKeyboardShortcuts()` for clearer constructor and easier maintenance.
- Animate button clicks for a single action codepath (Enter, button press, keyboard shortcuts).

### Changed
- `loadTextFromFile()` now stores the folder of the loaded file, clears the search input, and resets search/highlight state.
- Minor robustness and readability improvements (constants for defaults and filters, `isNewSearch()` helper).

### Notes
- UX and quality-of-life improvements only; no breaking API changes.

---

## [0.6.0] - 2025-10-10
- Load text files from disk via **Load** button, SearchOptions ctor/isValid, UI polish (clear button, window icon).

## [0.5.0] - 2025-10-10
- Prev/Next navigation, Enter-key trigger, Up/Down shortcuts, consistent iteration flags.

## [0.4.0] - 2025-10-10
- Highlight all matches, emphasize current match, press Enter to search.

## [0.3.0] - 2025-10-10
- Introduce SearchOptions abstraction to unify regex & find flags.

## [0.2.1] - 2025-10-05
- Add CHANGELOG.md and minor README updates (documentation-only patch).

## [0.2.0] - 2025-10-03
- Regex-based search (QRegularExpression), whole-word and case-sensitive options, match counting and status label.

## [0.1.0] - 2025-09-28
- Initial Qt tutorial-based import and baseline whole-word search.