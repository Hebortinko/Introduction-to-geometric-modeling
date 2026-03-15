# Introduction to Geometric Modeling

Welcome to my repository dedicated to the world of computer graphics.

## Purpose

The main goal of this project is to store my university assignments and track my personal progress in **Computer Graphics**. I hope this repo can serve as a learning resource or inspiration for fellow students and developers interested in how curves and shapes are built from scratch.

## Tech Stack

- **Language:** C++ (Standard 17/20)
- **Graphics Library:** [SFML 3.0](https://www.sfml-dev.org/documentation/3.0.2/)
- **Custom Library based on SFML:** [SimpleUI](https://github.com/Hebortinko/SimpleUI)
- **Build System:** Makefiles (Optimized for macOS / Apple Silicon)

## Build & Run

### macOS
Each assignment has its own `Makefile`. From inside an assignment directory:
```bash
make        # build
make run    # build and run
make clean  # remove build files
```
Requires SFML 3.0 installed via Homebrew: `brew install sfml`

### Windows (cross-compile from macOS)
1. Download **SFML 3 — GCC MinGW 64-bit** from [sfml-dev.org](https://www.sfml-dev.org/download)
2. Extract to `../SFML-win/` (next to assignment folders)
3. Install MinGW: `brew install mingw-w64`
4. From inside an assignment directory:
```bash
make -f Makefile.win SFML_DIR=../SFML-win
```

## Repository Structure

Each assignment is contained within its own directory with a dedicated `Makefile`:

- `Assignment 1/`: [Hermite's cube](https://github.com/Hebortinko/Introduction-to-geometric-modeling/tree/main/Assignment%201)
- `Assignment 2/`: ... 

*By the end of the semester, my ambition is to refactor these individual assignments and combime them into a **small modular "engine"** for comples shape creation and real-time manipulation*
