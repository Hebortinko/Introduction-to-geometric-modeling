# UILayer

Owns and manages all toolbar UI elements. Completely decoupled from application logic — communicates with `App` exclusively through callbacks.

## Responsibilities

- Creating and styling the `+`, `-`, and `Reset` buttons
- Rendering the LOD text label
- Positioning all elements relative to the window width (responsive)
- Forwarding `handleEvent` and `update` to all widgets
- Providing `isInUIZone()` so `App` can block clicks under the toolbar

## Callbacks

`UILayer` exposes three `std::function` callbacks that `App` sets in its constructor:

```cpp
m_ui->onPlus  = [this] { increaseLod(); };
m_ui->onMinus = [this] { decreaseLod(); };
m_ui->onReset = [this] { resetCanvas(); };
```

`UILayer` itself has no knowledge of `HermiteCube`, `state`, or any application logic. It only calls these functions when a button is clicked.

## Key Methods

| Method | Description |
|--------|-------------|
| `init(font)` | Creates `sf::Text` with the loaded font — must be called after font is ready |
| `handleEvent(event)` | Forwards SFML events to all buttons |
| `update(dt)` | Updates button animations |
| `draw(window)` | Positions and draws all UI elements |
| `setLodText(lod)` | Updates the LOD label string |
| `isInUIZone(pos, width)` | Returns true if `pos` is within the toolbar area |

## Layout

All positions are computed relative to the right edge of the window:

```
[Reset]   [LOD: 42]   [-]   [+]
w-225     w-160        w-93  w-48
```

This makes the toolbar responsive to window resizing.

## Initialization Order

`UILayer` must be constructed **after** `Theme::get().font` is set, because `Button` uses the font immediately on construction. For this reason `App` creates `UILayer` as a pointer (`new UILayer()`) inside the constructor body, not in the initializer list.

```cpp
// App.cpp
m_font.openFromFile(...);
Theme::get().font = &m_font;   // font ready
m_ui = new UILayer();          // safe to construct now
m_ui->init(m_font);            // sf::Text created here
```

## Dependencies

- `SimpleUI/Widgets/Button.h`
- `SimpleUI/Style/Presets.h`
