#pragma once
#include <SFML/Graphics.hpp>

class Widget
{
protected:
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f size = {0.f, 0.f};
    bool visible = true;
    bool enabled = true;

    virtual void onPositionChanged() {}
    virtual void onSizeChanged() {}

    bool containsMouse(sf::Vector2f mousePos) const
    {
        return getBounds().contains(mousePos);
    }

public:
    virtual ~Widget() = default;

    virtual void handleEvent(const sf::Event &event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;

    void setPosition(sf::Vector2f pos)
    {
        position = pos;
        onPositionChanged();
    }

    void setSize(sf::Vector2f sz)
    {
        size = sz;
        onSizeChanged();
    }

    void setVisible(bool v) { visible = v; }
    void setEnabled(bool e) { enabled = e; }

    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    bool isVisible() const { return visible; }
    bool isEnabled() const { return enabled; }

    sf::FloatRect getBounds() const
    {
        return sf::FloatRect(position, size);
    }
}; 