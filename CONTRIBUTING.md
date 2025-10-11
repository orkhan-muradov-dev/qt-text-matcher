# Contributing to TextMatcher

Thank you for considering contributing to TextMatcher! This document provides guidelines for contributing to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Development Guidelines](#development-guidelines)
- [Pull Request Process](#pull-request-process)

## Code of Conduct

### Our Standards

We are committed to providing a welcoming and inclusive environment. All participants are expected to:

- Use welcoming and inclusive language
- Be respectful of differing viewpoints
- Accept constructive criticism gracefully
- Focus on what is best for the community
- Show empathy towards others

### Reporting Issues

Report unacceptable behavior to orkhan.muradov.business@gmail.com.

## Getting Started

### Development Environment Setup

1. **Install dependencies**
   - Qt 6.0+ ([download](https://www.qt.io/download))
   - CMake 3.16+ ([download](https://cmake.org/download/))
   - C++17 compiler (GCC 8+, Clang 7+, MSVC 2017+)

2. **Clone and build**
   ```bash
   git clone https://github.com/YOUR_USERNAME/qt-text-matcher.git
   cd qt-text-matcher
   mkdir build && cd build
   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
   cmake --build .
   ```

3. **Run tests**
   ```bash
   ./TextMatcher
   # Verify basic functionality works
   ```

## How to Contribute

### Reporting Bugs

Before creating a bug report, search [existing issues](https://github.com/orkhan-muradov-dev/qt-text-matcher/issues) to avoid duplicates.

**Good bug reports include:**

```markdown
**Environment:**
- OS: Ubuntu 22.04 LTS
- Qt Version: 6.5.0
- Compiler: GCC 11.3.0

**Steps to Reproduce:**
1. Load file with 1000+ lines
2. Search for "test"
3. Click "Find Next" rapidly

**Expected Behavior:**
Smooth navigation through matches

**Actual Behavior:**
UI freezes for 2 seconds

**Additional Context:**
File size: 5MB, occurs consistently with large files
```

### Suggesting Features

Feature requests should include:

1. **Problem Statement** - What issue does this solve?
2. **Proposed Solution** - How would it work?
3. **Use Cases** - Who benefits and how?
4. **Alternatives** - What other approaches were considered?

**Example:**

```markdown
**Problem:** No way to save frequently used search patterns

**Solution:** Add search history dropdown (last 10 searches)

**Use Cases:**
- Developers checking recurring error patterns
- Writers verifying character name consistency

**Implementation:** Store in QSettings, add clear history option
```

### Contributing Code

Areas where we especially welcome contributions:

- 🐛 **Bug Fixes** - Check issues labeled `bug`
- 📚 **Documentation** - Improve clarity, add examples
- 🎨 **UI/UX** - Design improvements, accessibility
- ✨ **Features** - See [roadmap](README.md#-roadmap)
- 🧪 **Testing** - Add test coverage
- 🌍 **Translations** - Internationalization support

## Development Guidelines

### Code Style

**Naming Conventions:**

```cpp
// Classes: PascalCase
class TextMatcher {};

// Functions: camelCase
void performSearch();

// Variables: camelCase
QString searchText;

// Members: m_ prefix + camelCase
QString m_lastPattern;
size_t m_totalMatches;

// Constants: UPPER_SNAKE_CASE
constexpr auto DEFAULT_INPUT_FILE = ":/input.txt";
const QColor HighlightColor(100, 92, 17);
```

**Formatting:**

```cpp
// Indentation: 4 spaces
// Braces: Allman style (on new line)
void function()
{
    if (condition)
    {
        doSomething();
    }
    else
    {
        doSomethingElse();
    }
}

// Line length: aim for 100, hard limit 120

// Spacing
int result = a + b;
bool check = (x == y) && (z != w);
function(arg1, arg2);

// Pointers/references attached to type
QString* ptr;
const QString& ref;
```

**Comments:**

```cpp
// Single-line comments for brief notes
int count = 0;  // Track total matches

/* Multi-line comments for detailed
 * explanations spanning multiple lines.
 */

/// Doxygen-style for public API
/// @brief Performs text search
/// @param query Search pattern
/// @return Number of matches found
size_t search(const QString& query);
```

**Include Order:**

```cpp
// 1. Header for this source file
#include "textmatcher.h"

// 2. Qt includes
#include <QWidget>
#include <QString>

// 3. Standard library
#include <vector>
#include <algorithm>
```

**Modern C++ (C++17):**

```cpp
// Use auto for obvious types
auto text = ui->lineEdit->text();

// Range-based loops
for (const auto& match : matches) {
    process(match);
}

// nullptr instead of NULL
QWidget* widget = nullptr;

// const correctness
const QString& getText() const;

// Structured bindings (C++17)
if (auto [ok, value] = parse(text); ok) {
    use(value);
}
```

### Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <description>

<body>

<footer>
```

**Types:**
- `feat` - New feature
- `fix` - Bug fix
- `docs` - Documentation
- `style` - Formatting (no logic change)
- `refactor` - Code restructuring
- `perf` - Performance improvement
- `test` - Adding tests
- `chore` - Maintenance

**Scopes:** search, ui, highlight, file, build, docs

**Examples:**

```
feat(search): add regex mode toggle

Implemented direct regex input mode with UI toggle
and validation feedback.

Closes #42
```

```
fix(highlight): correct current match color

Changed from yellow to orange-brown for better
visual distinction.

Fixes #38
```

```
docs: update installation instructions

Added platform-specific steps for macOS M1 chips
and clarified Qt6 path configuration.
```

### Testing

**Manual Testing Checklist:**

Before submitting PR, verify:

- [ ] Basic search works
- [ ] Whole word option functions correctly
- [ ] Case sensitive toggle works
- [ ] Forward/backward navigation
- [ ] Wraparound at boundaries
- [ ] All matches highlighted
- [ ] Counter displays accurately
- [ ] File loading works
- [ ] All keyboard shortcuts respond
- [ ] Edge cases (empty search, no matches, single match)

**Test with:**
- Small files (<100 lines)
- Large files (1000+ lines)
- Edge cases (empty file, single line)

## Pull Request Process

### Before Submitting

**Checklist:**

- [ ] Code follows style guidelines
- [ ] Commits follow conventional format
- [ ] Self-reviewed code
- [ ] Added comments for complex logic
- [ ] Updated CHANGELOG.md under `[Unreleased]`
- [ ] Updated README.md if needed
- [ ] No new compiler warnings
- [ ] Tested on at least one platform
- [ ] Added tests if applicable

### PR Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation

## Testing
- [ ] Tested on [OS]
- [ ] Manual testing: [describe]
- [ ] All checklist items verified

## Related Issues
Fixes #123

## Checklist
- [ ] Code style followed
- [ ] Self-reviewed
- [ ] Documentation updated
- [ ] CHANGELOG.md updated
- [ ] No new warnings
```

### Review Process

1. Submit PR with clear description
2. Automated checks must pass (when implemented)
3. Address reviewer feedback
4. Maintainer approval required
5. Squash and merge

## Project Structure

```
TextMatcher/
├── 📄 README.md             # This file
├── 📄 CHANGELOG.md          # Version history
├── 📄 LICENSE               # MIT License
├── 📄 .gitignore            # Git ignore rules
│
├── 📁 src/                  # Source code
│   ├── 📄 CMakeLists.txt    # Build configuration
│   ├── 📝 main.cpp          # Application entry point
│   ├── 📝 textmatcher.h     # Class declaration
│   ├── 📝 textmatcher.cpp   # Implementation
│   ├── 📝 textmatcher.ui    # Qt Designer UI (XML)
│   ├── 📝 textmatcher.qrc   # Qt resources (embedded files)
│   └── 📄 input.txt         # Embedded sample text
│
└── 📁 media/                # Screenshots
```

### Key Components

- **SearchOptions** - Encapsulates regex, flags, options
- **State Management** - m_totalMatches, m_lastPattern, m_lastFolder
- **Search Algorithm** - Forward/backward with wraparound
- **Highlighting** - QTextEdit::ExtraSelection dual-color system

## Building and Testing

### Debug Build

```bash
mkdir build-debug && cd build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/path/to/Qt
cmake --build .
```

### With Warnings

```bash
cmake .. -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic"
```

### Qt Creator

1. Open CMakeLists.txt
2. Select Debug kit
3. Set breakpoints
4. Press F5 to debug

## Getting Help

- **Questions** - [GitHub Discussions](https://github.com/orkhan-muradov-dev/qt-text-matcher/discussions)
- **Issues** - [GitHub Issues](https://github.com/orkhan-muradov-dev/qt-text-matcher/issues)
- **Email** - orkhan.muradov.business@gmail.com

## Recognition

Contributors are acknowledged in:
- README.md contributors section
- Release notes
- GitHub contributors page

## License

By contributing, you agree contributions are licensed under the MIT License.

---

Thank you for contributing to TextMatcher! 🚀