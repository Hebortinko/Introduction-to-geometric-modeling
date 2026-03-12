# HermiteCube

Computes and renders a single uniform cubic Hermite curve from a control quadruple.

## Math

The curve is defined by four inputs:

```
R0  – start point
R1  – end point
r0  – tangent vector at R0  (passed as absolute position, App subtracts R0)
r1  – tangent vector at R1  (passed as absolute position, App subtracts R1)
```

The curve is evaluated using the **factored Hermite basis**:

```
H(t) = H00(t)·R0 + H10(t)·r0 + H01(t)·R1 + H11(t)·r1

H00(t) = (1 + 2t)(1 - t)²     – blend toward R0
H10(t) = t(1 - t)²            – tangent weight at R0
H01(t) = t²(3 - 2t)           – blend toward R1
H11(t) = t²(t - 1)            – tangent weight at R1
```

For `t ∈ [0, 1]`, sampled at `LOD` uniformly spaced points.

## Key Methods

| Method | Description |
|--------|-------------|
| `update(R0, R1, r0, r1)` | Recomputes all curve sample points |
| `draw(window)` | Renders the curve as thick line segments |
| `setLOD(lod)` | Sets number of sample points (minimum = DEFAULT_LOD) |
| `getLOD()` | Returns current LOD |
| `getDefaultLOD()` | Returns the minimum/default LOD constant |
| `setCurvePoints({})` | Clears the curve (used on reset) |
| `calculateBasis(t)` | Returns the 4 Hermite basis values for parameter t |

## Rendering

The curve is drawn as a series of `sf::RectangleShape` segments rotated to match each pair of adjacent sample points. This produces a smooth, thick line without relying on `LineStrip`.

```cpp
// Each segment:
angle  = atan2(diff.y, diff.x)
segment = RectangleShape({length, thickness})
segment.setRotation(sf::degrees(angle))
```

## LOD

`DEFAULT_LOD = 2` is the internal minimum — in practice the UI starts at 42 and allows incrementing/decrementing by 1. Lower LOD produces a visibly angular curve.
