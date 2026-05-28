# B-Spline Editor

Interactive editor for a 2D B-spline built with Rust, `egui`, and `eframe`.

## Features

- left click adds a new control point
- drag existing points to update the control polygon and sampled curve
- `Degree` slider changes the spline degree
- `Level Of Detail` slider changes sampling density
- knot vector input accepts comma-separated values
- validation reports missing values, invalid numbers, non-decreasing errors, and wrong knot count
- `Reset` restores the default state
- point and segment colors can be changed from the control panel

## Implementation Notes

The basis functions are evaluated recursively with the Cox-de Boor formula. The spline is sampled only over non-empty knot spans, and the editor validates that the knot vector length matches:

```text
control point count + degree + 1
```

## Project Structure

The assignment folder contains a short overview and the Rust project itself lives in `B-splajn/`:

```text
Assignment 5/
├── README.md
└── B-splajn/
    ├── Cargo.toml
    ├── Cargo.lock
    └── src/
        ├── main.rs
        ├── app.rs
        ├── spline_app.rs
        ├── b_spline.rs
        └── b_spline_basis_functions.rs
```

## Build & Run

Run from inside `Assignment 5/B-splajn/`:

```bash
cargo run
```

See the [root README](../README.md) for the repository overview.
