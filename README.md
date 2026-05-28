# Introduction to Geometric Modeling

Repository for my university assignments in geometric modeling and interactive computer graphics.

## Purpose

The goal of this project is to keep all semester assignments in one place, document how each editor works, and track the gradual evolution from standalone coursework into a more reusable graphics codebase.

## Tech Stack

- **Assignments 1-3:** C++17/20, [SFML 3.0](https://www.sfml-dev.org/documentation/3.0.2/), and my custom UI library [SimpleUI](https://github.com/Hebortinko/SimpleUI)
- **Assignments 4-5:** Rust 2024 with `egui` and `eframe`
- **Build Systems:** `make` for the C++ projects and Cargo for the Rust projects
- **Platform Focus:** macOS / Apple Silicon, with some Windows cross-build support

## Why Rust for Assignments 4 and 5?

Assignments 4 and 5 are intentionally written in Rust because I want to learn the language, it feels like a more modern direction, and it was a good challenge after spending a lot of time building custom widgets in earlier assignments.

My longer-term plan is to revisit the older assignments in Rust as well, improve the visuals, make the code more modular, and gradually shape the repository into a small CAD-like engine for geometric modeling.

## Build & Run

### Assignments 1-3 (C++ / SFML)

From inside an assignment directory:

```bash
make
make run
make clean
```

Requires SFML 3.0 installed via Homebrew:

```bash
brew install sfml
```

### Windows Cross-Compile for Assignments 1-3

1. Download **SFML 3 - GCC MinGW 64-bit** from [sfml-dev.org](https://www.sfml-dev.org/download)
2. Extract it to `../SFML-win/` next to the assignment folders
3. Install MinGW: `brew install mingw-w64`
4. Run from inside the selected assignment directory:

```bash
make -f Makefile.win SFML_DIR=../SFML-win
```

### Assignment 4 (Rust / egui)

Run from inside `Assignment 4/`:

```bash
cargo run
```

### Assignment 5 (Rust / egui)

Run from inside `Assignment 5/B-splajn/`:

```bash
cargo run
```

## Repository Structure

- `Assignment 1/`: [Hermite Cubic Curve Editor](./Assignment%201/README.md)
- `Assignment 2/`: [Bezier Curve Editor](./Assignment%202/README.md)
- `Assignment 3/`: [Hermite Spline Editor](./Assignment%203/README.md)
- `Assignment 4/`: [Cardinal Spline Editor](./Assignment%204/README.md)
- `Assignment 5/`: [B-Spline Editor](./Assignment%205/README.md)
- `SimpleUI/`: shared header-only UI library used by the C++ assignments

## Long-Term Direction

By the end of the semester, I want to refactor these individual assignments into a smaller modular system for interactive curve and shape creation, with the long-term goal of growing it into a compact CAD-like engine.
