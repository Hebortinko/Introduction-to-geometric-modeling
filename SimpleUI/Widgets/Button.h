#pragma once
#include "SimpleUI/Core/Widget.h"
#include "SimpleUI/Core/Theme.h"
#include "SimpleUI/Core/Event.h"
#include "SimpleUI/Style/WidgetStyle.h"
#include "SimpleUI/Style/Presets.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <optional>

class Button : public Widget {

private:
    sf::RectangleShape shape;
    std::optional<sf::Text> label;

    enum class State { Idle, Hovered, Pressed, Disabled };
    State state = State::Idle;

    sf::Color bgColor    = Theme::get().primary;
    sf::Color bgHover    = Theme::get().primaryHover;
    sf::Color bgPressed  = Theme::get().primaryPress;
    sf::Color txtColor   = sf::Color::White;
    sf::Color currentColor;

    Event<> onClickEvent;
    Event<> onHoverEvent;
    Event<> onPressEvent;

    void centerText() {
        if (!label) return;
        auto bounds = label->getLocalBounds();
        label->setOrigin(bounds.position + bounds.size / 2.f);
        label->setPosition(Widget::position + Widget::size / 2.f);
    }

    void onPositionChanged() override {
        shape.setPosition(Widget::position);
        centerText();
    }

    void onSizeChanged() override {
        shape.setSize(Widget::size);
        centerText();
    }

public:
    Button(const std::string& text, sf::Vector2f pos, sf::Vector2f sz) {
        setPosition(pos);
        setSize(sz);

        if (Theme::get().font) {
            label.emplace(*Theme::get().font);
            label->setString(text);
            label->setCharacterSize(Theme::get().fontSize);
            label->setFillColor(sf::Color::White);
        }

        currentColor = bgColor;
        shape.setFillColor(currentColor);
        centerText();
    }

    void handleEvent(const sf::Event& event) override {
        if (!enabled || !visible) return;

        if (auto* e = event.getIf<sf::Event::MouseMoved>()) {
            state = containsMouse(sf::Vector2f(e->position))
                    ? State::Hovered : State::Idle;
        }

        if (auto* e = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (state == State::Hovered) {
                state = State::Pressed;
                onPressEvent.emit();
            }
        }

        if (auto* e = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (state == State::Pressed) {
                onClickEvent.emit();
            }
            state = containsMouse(sf::Vector2f(e->position))
                    ? State::Hovered : State::Idle;
        }
    }

    void update(float dt) override {
        if (!enabled || !visible) return;

        sf::Color target;
        switch (state) {
            case State::Idle:     target = bgColor;                   break;
            case State::Hovered:  target = bgHover;                   break;
            case State::Pressed:  target = bgPressed;                 break;
            case State::Disabled: target = sf::Color(180, 180, 180); break;
        }

        float t = std::min(dt * 8.f, 1.f);
        currentColor = sf::Color(
            currentColor.r + (target.r - currentColor.r) * t,
            currentColor.g + (target.g - currentColor.g) * t,
            currentColor.b + (target.b - currentColor.b) * t
        );
        shape.setFillColor(currentColor);
    }

    void draw(sf::RenderWindow& window) const override {
        if (!visible) return;
        window.draw(shape);
        if (label) window.draw(*label);
    }

    void setText(const std::string& t) {
        if (label) { label->setString(t); centerText(); }
    }

    void setFontSize(unsigned int s) {
        if (label) { label->setCharacterSize(s); centerText(); }
    }

    void setDisabled(bool d) {
        enabled = !d;
        state = d ? State::Disabled : State::Idle;
    }

    Button& position(sf::Vector2f pos) {
        setPosition(pos);
        return *this;
    }

    Button& size(sf::Vector2f s) {
        setSize(s);
        return *this;
    }

    Button& text(const std::string& t) {
        setText(t);
        return *this;
    }

    Button& fontSize(unsigned int s) {
        setFontSize(s);
        return *this;
    }

    Button& disabled(bool d) {
        setDisabled(d);
        return *this;
    }

    Button& onClick(std::function<void()> cb) {
        onClickEvent.connect(cb);
        return *this;
    }

    Button& onHover(std::function<void()> cb) {
        onHoverEvent.connect(cb);
        return *this;
    }

    Button& onPress(std::function<void()> cb) {
        onPressEvent.connect(cb);
        return *this;
    }

    Button& style(ButtonStyle s) {
        if (s.bgColor)   { bgColor   = *s.bgColor; }
        if (s.bgHover)   { bgHover   = *s.bgHover; }
        if (s.bgPressed) { bgPressed = *s.bgPressed; }
        if (s.textColor) { txtColor  = *s.textColor; if (label) label->setFillColor(txtColor); }
        currentColor = bgColor;
        shape.setFillColor(currentColor);
        return *this;
    }
};