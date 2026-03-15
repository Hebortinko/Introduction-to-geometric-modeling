# Hermite Cubic Curve Editor

An interactive editor for drawing and manipulating a **uniform cubic Hermite curve** in 2D space, built with SFML 3.0.

---

## What is a Hermite Curve?

A cubic Hermite curve is defined by a **control quadruple** of four elements:

```
R0  – start point
R1  – end point
r0' – tangent vector at R0 (direction and speed of the curve at the start)
r1' – tangent vector at R1 (direction and speed of the curve at the end)
```

The curve is computed using the **factored (basis) form**:

```
H(t) = H00(t)·R0 + H10(t)·r0' + H01(t)·R1 + H11(t)·r1'
```

Where the Hermite basis functions are:

```
H00(t) = (1 + 2t)(1 - t)²
H10(t) = t(1 - t)²
H01(t) = t²(3 - 2t)
H11(t) = t²(t - 1)
```

For `t ∈ [0, 1]`, the curve is sampled at `LOD` uniformly spaced points and rendered as a polyline.

---

## How to Use

The application uses **click-based input** to define the control quadruple step by step:

| Step | Action | Result |
|------|--------|--------|
| 1 | Click anywhere | Places **R0** – start point (blue) |
| 2 | Click anywhere | Places **R1** – end point (blue) |
| 3 | Click anywhere | Places **r0** handle – tangent at R0 (red) |
| 4 | Click anywhere | Places **r1** handle – tangent at R1 (red) |

After all four points are placed, the curve is drawn automatically.

### Moving Control Points

Once placed, any control point can be **dragged interactively**:
- Click and hold on any point (blue or red)
- Drag to new position
- The curve updates **in real time** during dragging

### LOD Control

The **Level of Detail** controls how many sample points are used to draw the curve:

- `+` button – increase LOD by 10
- `-` button – decrease LOD by 10 (minimum: 42)
- Current LOD value is displayed next to the buttons

At low LOD values the curve appears angular. At higher values it becomes smooth.

---

## Visual Guide

```
        r0 (red handle)
        •
        |  ← tangent line
        |
R0 •────╯         ╭──── • R1
   (blue)    ~~~~╯      (blue)
                        |
                        • r1 (red handle)
```

---

## Project Structure

```
Assignment1/
├── main.cpp            – entry point
├── App.h / App.cpp     – application logic, input handling, rendering
├── HermiteCube.h       – curve definition and basis function declaration
├── HermiteCube.cpp     – curve computation and rendering implementation
└── Makefile
```

### Curve Implementation

The core curve logic lives in `HermiteCube.cpp`:
- `calculateBasis(t)` – computes the four Hermite basis functions for parameter t
- `update(...)` – samples the curve at LOD points and stores them
- `draw(...)` – renders the curve as thick line segments using `sf::RectangleShape`

---

## UI

The user interface is built on top of **SFML 3.0** using a custom UI library **SimpleUI** — a lightweight header-only widget library written from scratch for this project.

SimpleUI provides:
- `Button` widget with hover and press animations
- Theme system for consistent styling
- Method chaining API for clean widget configuration
- Style presets (Primary, Danger, Success, Ghost)

```cpp
// Example SimpleUI usage
btn.style(Presets::Success())
   .onClick([this] { m_lod += 10; });
```

SimpleUI is located in `../SimpleUI/` and is shared across assignments.

---

## Build & Run

See the [root README](../README.md) for build instructions.


---

## Controls Summary

| Input | Action |
|-------|--------|
| Left click (empty) | Place next control point |
| Left click + drag (on point) | Move control point |
| `+` button | Increase LOD |
| `-` button | Decrease LOD |
