# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog** and this project follows [Semantic Versioning](https://semver.org/).

## Unreleased
- (Work in progress)

---

## [0.2.1] - 2025-10-05
### Added
- Add CHANGELOG.md to track releases and project history.
- Minor documentation updates (README) and release metadata.

### Notes
- v0.2.1 is a documentation-only patch that records the changelog and README updates.
- No behavior changes in the application code for this patch.

---

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
