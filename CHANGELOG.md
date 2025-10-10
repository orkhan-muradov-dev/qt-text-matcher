# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog** and this project follows Semantic Versioning.

## Unreleased
- (Work in progress)

---

## [0.5.0] - 2025-10-10
### Added
- Prev/Next navigation controls (UI buttons) to move between matches.
- `performFind(bool backwards)` to support forward/backward search with consistent behavior.
- Keyboard shortcuts: **Up** = previous match, **Down** = next match (QShortcut).
- Press **Enter** in the search field to trigger Find Next (convenience UX).

### Changed
- Counting and highlighting continue to iterate forward for lists (use `iterateFlagsFor`) while the search direction can be backwards for navigation.
- Minor UX polish to keep highlights and status display in sync when navigating.

### Notes
- This is a UX-focused minor release (no breaking API changes). The code is prepared for future features like raw-regex mode and multi-file scanning.

---

## [0.4.0] - 2025-10-10
### Added
- Highlight all matches in the QTextEdit using `QTextEdit::ExtraSelection`.
- Visual emphasis for the current match.
- Press Enter in the search input to trigger a search.

### Changed
- `resetSearchState()` clears extra selections to remove highlights on reset.

---

## [0.3.0] - 2025-10-10
### Added
- Introduce `SearchOptions` abstraction (regex, pattern options, and QTextDocument::FindFlags).

### Changed
- Refactor search logic: count and index functions accept `SearchOptions`.

---

## [0.2.1] - 2025-10-05
- Add CHANGELOG.md and minor README updates (documentation-only patch).

## [0.2.0] - 2025-10-03
- Regex-based search (QRegularExpression), whole-word and case-sensitive options,
  match counting and status label, refactor to TextMatcher.

## [0.1.0] - 2025-09-28
- Initial Qt tutorial-based import and baseline whole-word search.