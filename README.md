# Qt Text Matcher

A small Qt Widgets application for searching and matching text inside files.
Started from the Qt "Text Finder" tutorial and evolved into a local text-matching utility.

## Features
- Simple Qt Widgets UI (line edit + find button + QTextEdit)
- Loads initial text from resource (input.txt)
- Whole-word search with wrap-around

## Build (CMake + Qt)
Requires Qt 5 or Qt 6 and CMake >= 3.16.

```bash
mkdir build
cd build
cmake ..
cmake --build .
./TextFinder
