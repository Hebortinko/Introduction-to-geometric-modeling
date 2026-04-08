# Bezier Curve Editor

An interactive editor for drawing and manipulating a **Bezier curve** in 2D space, built with SFML 3.0.

---

## What is a Bezier Curve?

A Bezier curve is defined by a sequence of **control points**. The curve does not usually pass through all of them, but their positions determine its overall shape.

This project evaluates the curve using the **de Casteljau algorithm**:

1. Interpolate between neighboring control points for a selected parameter `t`
2. Repeat the interpolation on the newly created points
3. Continue until only one point remains
4. That final point lies on the Bezier curve

The app also visualizes the intermediate de Casteljau levels, so you can see how the construction works for the current value of `t`.

---

## How to Use

The application uses a simple point-based workflow:

| Step | Action | Result |
|------|--------|--------|
| 1 | Left click on empty canvas | Places the **first control point** |
| 2 | Left click again | Adds another control point |
| 3 | Continue left clicking | Appends more control points to the polygon |

The **first click creates the first point**, and every next left click adds a new point, so the **last clicked point becomes the current last control point** of the control polygon.

### Moving Control Points

Existing control points can be repositioned at any time:

- **Right click and hold** on a point
- Drag it to a new position
- Release the right mouse button to drop it

The Bezier curve, control polygon, and de Casteljau visualization update immediately while dragging.

### LOD Control

The **Level of Detail** determines how many sampled points are used to draw the curve:

- `+` button – increase LOD by 1
- `-` button – decrease LOD by 1
- Minimum LOD is `42`

Higher LOD produces a smoother rendered curve.

### Parameter t

The slider at the bottom controls the parameter `t` in the interval `[0, 1]`.

- The app shows intermediate de Casteljau levels for the selected `t`
- The yellow point marks the current point on the Bezier curve
- The label above the slider displays the current `t` value

### Reset

- `Reset` clears all control points
- LOD is restored to its default value
- The slider returns to its default position

---

## Visual Guide

```text
P0 •────• P1 ────• P2 ────• P3
 \                         /
  \__ sampled Bezier ____/

Green helper lines show intermediate de Casteljau levels
Yellow point shows the curve position for the selected t
```

---

## Project Structure

```text
Assignment 2/
├── main.cpp               – entry point
├── App/App.h / App.cpp    – application loop, input handling, rendering
├── BeziersCube/Beziers.h  – Bezier curve interface
├── BeziersCube/Beziers.cpp– de Casteljau evaluation and curve sampling
├── UI/UILayer.h / .cpp    – buttons, slider, and UI layout
└── Makefile / Makefile.win
```

### Curve Implementation

The core curve logic lives in `BeziersCube/Beziers.cpp`:

- `deCasteljauLevels(t)` – computes all interpolation levels for parameter `t`
- `evaluate(t)` – returns the final point on the curve
- `sampleCurve()` – samples the full curve using the current LOD

---

## UI

The user interface is built on top of **SFML 3.0** using the custom header-only library **SimpleUI**.

It provides:

- `+` and `-` buttons for LOD adjustment
- `Reset` button for clearing the canvas
- A slider for controlling the de Casteljau parameter `t`

SimpleUI is shared across assignments and located in `../SimpleUI/`.

---

## Build & Run

### macOS

```bash
make
./bezier_cube
```

### Windows Cross-Compile from macOS

Run from inside `Assignment 2/`:

```bash
make -f Makefile.win SFML_DIR=../SFML-win CXXFLAGS='-I../SFML-win/include -I.. -std=c++20 -DSFML_STATIC'
```

This produces:

```text
beziers.exe
```

---

## Controls Summary

| Input | Action |
|-------|--------|
| Left click | Add a new control point |
| Right click + drag | Move an existing control point |
| Slider | Change parameter `t` |
| `+` button | Increase LOD |
| `-` button | Decrease LOD |
| `Reset` button | Clear all points and restore defaults |
