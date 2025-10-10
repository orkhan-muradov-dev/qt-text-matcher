# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog** and this project follows Semantic Versioning.

## Unreleased
- (Work in progress)

---

## [0.6.0] - 2025-10-10
### Added
- Load text files from disk via **Load** button (native QFileDialog).
- `loadTextFromFile(path)` to safely stream file contents into the QTextEdit.
- `SearchOptions` constructor and `isValid()` to centralize regex and find-flag construction.
- Clear button enabled on the search field for quick clearing.
- Window icon set using platform-style standard icon.
- Highlight color constants (`HighlightColor`, `CurrentHighlightColor`) extracted for easier tuning.

### Changed
- Loading a file clears the search box, resets cursor and search state to avoid stale highlights/counts.
- Minor UI polish and robustness improvements.

### Notes
- This is a minor feature release (UX + I/O). No breaking API changes.

---

## [0.5.0] - 2025-10-10
### Added
- Prev/Next navigation controls (UI buttons) to move between matches.
- `performFind(bool backwards)` to support forward/backward search with consistent behavior.
- Keyboard shortcuts: **Up** = previous, **Down** = next (QShortcut).
- Press **Enter** in the search field to trigger Find Next.

### Changed
- Counting and highlighting iterate forward when building lists (iterateFlagsFor),
  while search navigation may run backwards for match traversal.

---

## [0.4.0] - 2025-10-10
### Added
- Highlight all matches using `QTextEdit::ExtraSelection`.
- Visual emphasis for current match.
- Press Enter to trigger search.

### Changed
- `resetSearchState()` clears extra selections on reset.

---

## [0.3.0] - 2025-10-10
### Added
- `SearchOptions` abstraction (regex, pattern options, and QTextDocument::FindFlags) to unify behavior.

---

## [0.2.1] - 2025-10-05
- Add CHANGELOG.md and minor README updates (documentation-only patch).

## [0.2.0] - 2025-10-03
- Regex-based search (QRegularExpression), whole-word and case-sensitive options,
  match counting and status label, refactor to TextMatcher.

## [0.1.0] - 2025-09-28
- Initial Qt tutorial-based import with basic whole-word search and resource loading from `:/input.txt`.