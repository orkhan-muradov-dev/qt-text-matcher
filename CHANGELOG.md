# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog** and this project follows Semantic Versioning.

## Unreleased
- (Work in progress)

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
- This is a minor feature release that improves correctness and lays groundwork for
  future features (raw-regex mode, multi-file scanning, threaded search).

---

## [0.2.1] - 2025-10-05
- Add CHANGELOG.md and minor README updates (documentation-only patch).

## [0.2.0] - 2025-10-03
- Regex-based search (QRegularExpression), whole-word and case-sensitive options,
  match counting and status label, refactor to TextMatcher.

## [0.1.0] - 2025-09-28
- Initial Qt tutorial-based import and baseline whole-word search.