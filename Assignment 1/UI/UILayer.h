#pragma once
#include "../SimpleUI/Widgets/Button.h"
#include "../SimpleUI/Style/Presets.h"
#include <SFML/Graphics.hpp>
#include <functional>

class UILayer
{
public:
    std::function<void()> onPlus;
    std::function<void()> onMinus;
    std::function<void()> onReset;

    UILayer();
    ~UILayer();

    void init(sf::Font& font);
    void handleEvent(const sf::Event &event);
    void update(float dt);
    void draw(sf::RenderWindow &window);
    void setLodText(int lod);
    bool isInUIZone(sf::Vector2f pos, float windowWidth);

private:
    Button *m_btnPlus = nullptr;
    Button *m_btnMinus = nullptr;
    Button *m_btnReset = nullptr;
    sf::Text *m_lodText = nullptr;
};