# App

The main application class. Owns the SFML window, manages application state, handles user input, and coordinates between `HermiteCube` and `UILayer`.

## Responsibilities

- Creating and running the main loop
- Managing the 4-step placement state machine (R0 → R1 → r0 → r1)
- Handling mouse input (click to place, drag to move)
- Rendering control points and tangent lines
- Delegating UI events and LOD changes to the appropriate classes

## State Machine

The application progresses through states as the user clicks:

```
state 0 → click → place R0  (start point)
state 1 → click → place R1  (end point)
state 2 → click → place r0  (tangent handle at R0)
state 3 → click → place r1  (tangent handle at R1)
state 4 → curve fully defined, drag any point to edit
```

## Key Methods

| Method | Description |
|--------|-------------|
| `run()` | Starts the main loop |
| `handleInput(event)` | Processes mouse clicks and drag |
| `onMouseClick(pos)` | Advances the state machine on click |
| `drawScene()` | Renders points, handles and tangent lines |
| `increaseLod()` / `decreaseLod()` | Adjusts curve resolution |
| `resetCanvas()` | Resets state to 0 and clears the curve |
| `isMouseOver(mouse, point)` | Hit test for draggable points |
| `isInUIZone(pos)` | Prevents placing points under the toolbar |

## Dependencies

- `HermiteCube` – computes and draws the curve
- `UILayer` – owns all buttons and toolbar rendering
- `SimpleUI/Theme` – must have font set before UILayer is constructed
