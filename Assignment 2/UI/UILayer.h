#pragma once
#include "SimpleUI/Widgets/Button.h"
#include "SimpleUI/Style/Presets.h"
#include "SimpleUI/Widgets/Slider.h"
#include <SFML/Graphics.hpp>
#include <functional>

class UILayer
{
public:
    std::function<void()> onPlus;
    std::function<void()> onMinus;
    std::function<void()> onReset;
    Slider<float> slider;

    UILayer();
    ~UILayer();

    void init(sf::Font& font);
    void handleEvent(const sf::Event &event);
    void update(float dt);
    void draw(sf::RenderWindow &window);
    void setLodText(int lod);
    bool isInUIZone(sf::Vector2f pos, float windowWidth);
    void setSliderValue(float newVal);
    

private:
    Button *m_btnPlus = nullptr;
    Button *m_btnMinus = nullptr;
    Button *m_btnReset = nullptr;
    sf::Text *m_lodText    = nullptr;
    sf::Text *m_sliderText = nullptr;
};