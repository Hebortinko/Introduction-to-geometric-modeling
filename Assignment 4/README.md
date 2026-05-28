# Cardinal Spline Editor

Interactive editor for a 2D cardinal spline built with Rust, `egui`, and `eframe`.

## Features

- left click adds a new control point
- drag existing points to update the spline in real time
- `Shape Parameter` slider changes the spline tension
- `Level Of Detail` slider changes sampling density
- `Interpolate Ends` duplicates the first and last control point so the spline passes through both ends
- the right panel visualizes the four cardinal basis functions for the current shape parameter
- `Reset` restores the default state
- point and segment colors can be changed from the control panel

## Implementation Notes

Each segment is evaluated in cubic Hermite form. For a local control quadruple `P0, P1, P2, P3`, the tangents are computed as:

```text
T1 = s (P2 - P0)
T2 = s (P3 - P1)
```

Where `s` is the current shape parameter.

## Project Structure

```text
Assignment 4/
├── .cargo/config.toml
├── Cargo.toml
├── Cargo.lock
└── src/
    ├── main.rs
    ├── app.rs
    ├── spline_app.rs
    ├── cardinal_spline.rs
    ├── cardinal_spline_basis_functions.rs
    └── hermite.rs
```

## Build & Run

Run from inside `Assignment 4/`:

```bash
cargo run
```

See the [root README](../README.md) for the repository overview.
