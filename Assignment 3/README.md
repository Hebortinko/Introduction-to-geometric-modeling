# Hermite Spline Editor

Interactive editor for a piecewise cubic uniform Hermite spline in 2D, built with SFML 3.0.

## Implemented Boundary Condition

This assignment uses the natural boundary condition for the endpoint tangent vectors.

For control points `P0, ..., Pk` and tangent vectors `r0, ..., rk`, the solved linear system is:

```text
2 r0 + r1       = 3 (P1 - P0)
r(i-1) + 4 ri + r(i+1) = 3 (P(i+1) - P(i-1)),  i = 1, ..., k-1
r(k-1) + 2 rk   = 3 (Pk - P(k-1))
```

The spline is then rendered segment-by-segment in cubic Hermite form.

## Features

- left click adds a new control point
- right click and drag moves an existing control point
- each spline segment is rendered in a different color
- each control point is numbered
- each computed tangent vector is visualized as a normalized arrow showing direction
- `+` and `-` change LOD
- `Reset` clears the scene and restores defaults

## Project Structure

```text
Assignment 3/
├── main.cpp
├── App/
├── HermiteSpline/
├── Math/
├── UI/
├── assets/
├── external/eigen/
├── Makefile
├── Makefile.win
└── hermite_spline.exe
```

`SimpleUI/` is included next to `Assignment 3/` in the delivery archive because the UI layer depends on it.

## Build

### macOS

```bash
make
./hermite_spline
```

### Windows Cross-Compile from macOS

Run from inside `Assignment 3/`:

```bash
make -f Makefile.win SFML_DIR=../SFML-win CXXFLAGS='-I../SFML-win/include -I.. -Iexternal/eigen -std=c++20 -DSFML_STATIC'
```

This produces:

```text
hermite_spline.exe
```
