#pragma once
#include "SimpleUI/Core/Widget.h"
#include "SimpleUI/Core/Theme.h"
#include "SimpleUI/Core/Event.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <type_traits>

template<typename T>
class Slider : public Widget {
    static_assert(std::is_same_v<T, int> || std::is_same_v<T, float>,
                  "Slider supports only int or float");

private:
    T minimum;
    T maximum;
    T step;
    T currentValue;

    sf::RectangleShape track;
    sf::RectangleShape fill;
    sf::CircleShape knob;

    bool dragging = false;

    sf::Color trackColorValue = Theme::get().border;
    sf::Color fillColor = Theme::get().primary;
    sf::Color knobColorValue = Theme::get().primaryHover;
    sf::Color outlineColor = Theme::get().primaryPress;

    Event<T> onChangeEvent;

    static T defaultStep() {
        if constexpr (std::is_same_v<T, int>) {
            return 1;
        } else {
            return static_cast<T>(0.01f);
        }
    }

    static bool isSameValue(T lhs, T rhs) {
        if constexpr (std::is_same_v<T, int>) {
            return lhs == rhs;
        } else {
            return std::abs(lhs - rhs) < static_cast<T>(0.0001f);
        }
    }

    void sanitizeRange() {
        if (maximum < minimum) {
            std::swap(minimum, maximum);
        }

        if (step <= static_cast<T>(0)) {
            step = defaultStep();
        }

        currentValue = snapValue(currentValue);
    }

    float getNormalizedValue() const {
        const double range = static_cast<double>(maximum) - static_cast<double>(minimum);
        if (range <= 0.0) return 0.f;

        const double offset = static_cast<double>(currentValue) - static_cast<double>(minimum);
        return static_cast<float>(std::clamp(offset / range, 0.0, 1.0));
    }

    T castValue(double rawValue) const {
        if constexpr (std::is_same_v<T, int>) {
            return static_cast<T>(std::lround(rawValue));
        } else {
            return static_cast<T>(rawValue);
        }
    }

    T snapValue(T rawValue) const {
        const double minValue = static_cast<double>(minimum);
        const double maxValue = static_cast<double>(maximum);
        const double clamped = std::clamp(static_cast<double>(rawValue), minValue, maxValue);
        const double range = maxValue - minValue;

        if (range <= 0.0) {
            return minimum;
        }

        const double stepValue = std::max(static_cast<double>(step), static_cast<double>(defaultStep()));
        const std::size_t count = std::max<std::size_t>(
            1,
            static_cast<std::size_t>(std::ceil(range / stepValue))
        );

        double closest = minValue;
        double bestDistance = std::numeric_limits<double>::max();

        for (std::size_t i = 0; i <= count; ++i) {
            double candidate = minValue + stepValue * static_cast<double>(i);
            if (candidate > maxValue || i == count) {
                candidate = maxValue;
            }

            const double distance = std::abs(candidate - clamped);
            if (distance < bestDistance) {
                bestDistance = distance;
                closest = candidate;
            }
        }

        return castValue(closest);
    }

    T valueFromMouse(float mouseX) const {
        const float width = track.getSize().x;
        if (width <= 0.f) return minimum;

        const float left = track.getPosition().x;
        const float ratio = std::clamp((mouseX - left) / width, 0.f, 1.f);
        const double rawValue = static_cast<double>(minimum) +
            (static_cast<double>(maximum) - static_cast<double>(minimum)) * static_cast<double>(ratio);

        return snapValue(castValue(rawValue));
    }

    void updateColors() {
        track.setFillColor(trackColorValue);
        track.setOutlineThickness(1.f);
        track.setOutlineColor(outlineColor);

        fill.setFillColor(fillColor);

        knob.setFillColor(knobColorValue);
        knob.setOutlineThickness(1.f);
        knob.setOutlineColor(outlineColor);
        knob.setPointCount(32);
    }

    void updateGeometry() {
        const float safeWidth = std::max(Widget::size.x, 0.f);
        const float safeHeight = std::max(Widget::size.y, 0.f);

        if (safeWidth <= 0.f || safeHeight <= 0.f) {
            track.setPosition(Widget::position);
            track.setSize({0.f, 0.f});
            fill.setPosition(Widget::position);
            fill.setSize({0.f, 0.f});
            knob.setRadius(0.f);
            knob.setOrigin({0.f, 0.f});
            knob.setPosition(Widget::position);
            return;
        }

        float trackHeight = std::max(4.f, safeHeight * 0.35f);
        trackHeight = std::min(trackHeight, safeHeight);
        const float knobDiameter = std::min(std::max(trackHeight * 1.5f, safeHeight * 0.65f), safeWidth);
        const float knobRadius = knobDiameter * 0.5f;
        const float centerY = Widget::position.y + safeHeight * 0.5f;
        const float trackX = Widget::position.x + knobRadius;
        const float trackWidth = std::max(safeWidth - knobDiameter, 0.f);
        const float trackY = centerY - trackHeight * 0.5f;
        const float normalized = getNormalizedValue();

        track.setPosition({trackX, trackY});
        track.setSize({trackWidth, trackHeight});

        fill.setPosition(track.getPosition());
        fill.setSize({trackWidth * normalized, trackHeight});

        knob.setRadius(knobRadius);
        knob.setOrigin({knobRadius, knobRadius});
        knob.setPosition({
            trackX + trackWidth * normalized,
            centerY
        });
    }

    void applyValue(T newValue, bool emitChange) {
        const T snappedValue = snapValue(newValue);
        const bool changed = !isSameValue(currentValue, snappedValue);

        currentValue = snappedValue;
        updateGeometry();

        if (changed && emitChange) {
            onChangeEvent.emit(currentValue);
        }
    }

    void onPositionChanged() override {
        updateGeometry();
    }

    void onSizeChanged() override {
        updateGeometry();
    }

public:
    Slider(T min, T max, T stepValue, sf::Vector2f pos, sf::Vector2f sz)
        : minimum(min), maximum(max), step(stepValue), currentValue(min) {
        sanitizeRange();
        updateColors();
        setPosition(pos);
        setSize(sz);
        updateGeometry();
    }

    Slider(T min, T max, T stepValue, sf::Vector2f pos, sf::Vector2f sz, T initialValue)
        : minimum(min), maximum(max), step(stepValue), currentValue(initialValue) {
        sanitizeRange();
        updateColors();
        setPosition(pos);
        setSize(sz);
        updateGeometry();
    }

    void handleEvent(const sf::Event& event) override {
        if (!enabled || !visible) return;

        if (auto* e = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (e->button == sf::Mouse::Button::Left && containsMouse(sf::Vector2f(e->position))) {
                dragging = true;
                applyValue(valueFromMouse(static_cast<float>(e->position.x)), true);
            }
        }

        if (auto* e = event.getIf<sf::Event::MouseMoved>()) {
            if (dragging) {
                applyValue(valueFromMouse(static_cast<float>(e->position.x)), true);
            }
        }

        if (auto* e = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (e->button == sf::Mouse::Button::Left) {
                dragging = false;
            }
        }
    }

    void update(float dt) override {
        (void)dt;
    }

    void draw(sf::RenderWindow& window) const override {
        if (!visible) return;
        window.draw(track);
        window.draw(fill);
        window.draw(knob);
    }

    T getValue() const { return currentValue; }
    T getMinimum() const { return minimum; }
    T getMaximum() const { return maximum; }
    T getStep() const { return step; }

    void setValue(T newValue) {
        applyValue(newValue, true);
    }

    void setRange(T min, T max) {
        minimum = min;
        maximum = max;
        sanitizeRange();
        updateGeometry();
    }

    void setStep(T stepValue) {
        step = stepValue;
        sanitizeRange();
        updateGeometry();
    }

    void setColor(sf::Color color) {
        fillColor = color;
        knobColorValue = color;
        outlineColor = color;
        updateColors();
    }

    void setTrackColor(sf::Color color) {
        trackColorValue = color;
        updateColors();
    }

    void setKnobColor(sf::Color color) {
        knobColorValue = color;
        updateColors();
    }

    Slider& position(sf::Vector2f pos) {
        setPosition(pos);
        return *this;
    }

    Slider& size(sf::Vector2f sz) {
        setSize(sz);
        return *this;
    }

    Slider& range(T min, T max) {
        setRange(min, max);
        return *this;
    }

    Slider& increment(T stepValue) {
        setStep(stepValue);
        return *this;
    }

    Slider& value(T newValue) {
        setValue(newValue);
        return *this;
    }

    Slider& color(sf::Color colorValue) {
        setColor(colorValue);
        return *this;
    }

    Slider& trackColor(sf::Color colorValue) {
        setTrackColor(colorValue);
        return *this;
    }

    Slider& knobColor(sf::Color colorValue) {
        setKnobColor(colorValue);
        return *this;
    }

    Slider& onChange(std::function<void(T)> cb) {
        onChangeEvent.connect(cb);
        return *this;
    }
};
