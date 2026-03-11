#pragma once
#include "Core/Widget.h"
#include "Core/Theme.h"
#include "Core/Event.h"
#include "Style/WidgetStyle.h"
#include "Style/Presets.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button : public Widget
{

private:
    sf::RectangleShape shape;
    sf::Text label;

    enum class State
    {
        Idle,
        Hovered,
        Pressed,
        Disabled
    };
    State state = State::Idle;

    
    sf::Color m_bgColor = Theme::get().primary;
    sf::Color m_bgHover = Theme::get().primaryHover;
    sf::Color m_bgPressed = Theme::get().primaryPress;
    sf::Color m_txtColor = sf::Color::White;
    sf::Color m_currentColor;

    
    Event<> onClickEvent;
    Event<> onHoverEvent;
    Event<> onPressEvent;

    void centerText()
    {
        auto bounds = label.getLocalBounds();
        label.setOrigin(bounds.position + bounds.size / 2.f);
        label.setPosition(position + size / 2.f);
    }

    void onPositionChanged() override
    {
        shape.setPosition(position);
        centerText();
    }

    void onSizeChanged() override
    {
        shape.setSize(size);
        centerText();
    }

public:
    
    Button(const std::string &text, sf::Vector2f pos, sf::Vector2f sz)
        : label(*Theme::get().font) 
    {
        setPosition(pos);
        setSize(sz);

        label.setString(text);
        label.setCharacterSize(Theme::get().fontSize);
        label.setFillColor(sf::Color::White);

        m_currentColor = m_bgColor;
        shape.setFillColor(m_currentColor);
        centerText();
    }

    
    void handleEvent(const sf::Event &event) override
    {
        if (!enabled || !visible)
            return;

        if (auto *e = event.getIf<sf::Event::MouseMoved>())
        {
            state = containsMouse(sf::Vector2f(e->position))
                        ? State::Hovered
                        : State::Idle;
        }

        if (auto *e = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (state == State::Hovered)
            {
                state = State::Pressed;
                onPressEvent.emit();
            }
        }

        if (auto *e = event.getIf<sf::Event::MouseButtonReleased>())
        {
            if (state == State::Pressed)
            {
                onClickEvent.emit();
            }
            state = containsMouse(sf::Vector2f(e->position))
                        ? State::Hovered
                        : State::Idle;
        }
    }

    void update(float dt) override
    {
        if (!enabled || !visible)
            return;

        sf::Color target;
        switch (state)
        {
        case State::Idle:
            target = m_bgColor;
            break;
        case State::Hovered:
            target = m_bgHover;
            break;
        case State::Pressed:
            target = m_bgPressed;
            break;
        case State::Disabled:
            target = sf::Color(180, 180, 180);
            break;
        }

        float t = std::min(dt * 8.f, 1.f);
        m_currentColor = sf::Color(
            m_currentColor.r + (target.r - m_currentColor.r) * t,
            m_currentColor.g + (target.g - m_currentColor.g) * t,
            m_currentColor.b + (target.b - m_currentColor.b) * t);
        shape.setFillColor(m_currentColor);
    }

    void draw(sf::RenderWindow &window) const override
    {
        if (!visible)
            return;
        window.draw(shape);
        window.draw(label);
    }

    
    void setText(const std::string &t)
    {
        label.setString(t);
        centerText();
    }

    void setFontSize(unsigned int s)
    {
        label.setCharacterSize(s);
        centerText();
    }

    void setDisabled(bool d)
    {
        enabled = !d;
        state = d ? State::Disabled : State::Idle;
    }


    Button &onClick(std::function<void()> cb)
    {
        onClickEvent.connect(cb);
        return *this;
    }

    Button &onHover(std::function<void()> cb)
    {
        onHoverEvent.connect(cb);
        return *this;
    }

    Button &onPress(std::function<void()> cb)
    {
        onPressEvent.connect(cb);
        return *this;
    }

    Button &style(ButtonStyle s)
    {
        if (s.bgColor)
        {
            m_bgColor = *s.bgColor;
        }
        if (s.bgHover)
        {
            m_bgHover = *s.bgHover;
        }
        if (s.bgPressed)
        {
            m_bgPressed = *s.bgPressed;
        }
        if (s.textColor)
        {
            m_txtColor = *s.textColor;
            label.setFillColor(m_txtColor);
        }
        m_currentColor = m_bgColor;
        shape.setFillColor(m_currentColor);
        return *this;
    }

    Button &disabled(bool d)
    {
        setDisabled(d);
        return *this;
    }
};
