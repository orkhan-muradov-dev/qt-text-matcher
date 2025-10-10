# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog** and this project follows Semantic Versioning.

## Unreleased
- (Work in progress)

---

## [0.4.0] - 2025-10-10
### Added
- Highlight all matches in the QTextEdit using `QTextEdit::ExtraSelection` with `highlightAllMatches()`.
- Visual emphasis for the current match so users can quickly locate it in context.
- Press **Enter** in the search input to trigger a search (connected `returnPressed` to the search handler).

### Changed
- `resetSearchState()` now clears extra selections to remove highlights on reset.
- Minor UX improvements: synchronized highlighting and match-count updates.

### Notes
- This is a UX-focused minor release (no breaking API changes). Colors used for highlighting are simple defaults and can be tuned for better contrast or accessibility.

---

## [0.3.0] - 2025-10-10
### Added
- Introduce `SearchOptions` abstraction (regex, pattern options, and QTextDocument::FindFlags)
  to unify search/selection behavior.
- `buildSearchOptions()` centralizes pattern creation and flags so case sensitivity and
  whole-word settings are applied consistently to both highlighting and counting.

### Changed
- Refactor search logic: countMatches/calculateTotalMatches/calculateCurrentMatchIndex
  accept `SearchOptions` to avoid duplicated logic and ensure consistent behavior.
- Minor robustness improvements in loadTextFile() and regex validation.

### Notes
- Minor feature release to improve correctness and lay groundwork for future features.

---

## [0.2.1] - 2025-10-05
- Add CHANGELOG.md and minor README updates (documentation-only patch).

## [0.2.0] - 2025-10-03
### Added
- Regex-based search using `QRegularExpression` with input escaping for safe defaults.
- UI options: **Whole Word** checkbox (wraps the escaped input with `\b` anchors) and **Case Sensitive** toggle.
- Match counting and a persistent status label showing `Matches: current/total`.
- Validation handling for invalid regex patterns with graceful reset behavior.

### Changed
- Refactor: rename `TextFinder` → `TextMatcher`, slot rename to `handleFindClicked`, and explicit `connect()` usage for the Find button.
- Search logic now caches last regex string and pattern options to avoid redundant total-match recalculation.

### Fixed
- Clear selection and reset state when search input is empty or invalid to keep UI consistent.

### Notes
- By default user input is escaped to treat special regex characters literally. If you want to accept raw regex from users, consider adding an “Advanced: raw regex” option and documenting it in the README.

---

## [0.1.0] - 2025-09-28
- Initial Qt tutorial-based import with basic whole-word search and resource loading from `:/input.txt`.