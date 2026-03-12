#include "UILayer.h"

UILayer::UILayer()
{
    m_btnPlus  = new Button("+",     {0.f, 0.f}, {40.f, 35.f});
    m_btnMinus = new Button("-",     {0.f, 0.f}, {40.f, 35.f});
    m_btnReset = new Button("Reset", {0.f, 0.f}, {55.f, 35.f});
    // m_lodText sa vytvorí až v init()

    m_btnPlus->style(Presets::Success());
    m_btnMinus->style(Presets::Danger());
    m_btnReset->style(Presets::Ghost());

    m_btnPlus->onClick( [this] { if (onPlus)  onPlus();  });
    m_btnMinus->onClick([this] { if (onMinus) onMinus(); });
    m_btnReset->onClick([this] { if (onReset) onReset(); });
}

UILayer::~UILayer()
{
    delete m_btnPlus;
    delete m_btnMinus;
    delete m_btnReset;
    delete m_lodText;
}

void UILayer::init(sf::Font& font)
{
    m_lodText = new sf::Text(font); 
    m_lodText->setCharacterSize(16);
    m_lodText->setFillColor(sf::Color::Black);
}

void UILayer::handleEvent(const sf::Event &event)
{
    m_btnPlus->handleEvent(event);
    m_btnMinus->handleEvent(event);
    m_btnReset->handleEvent(event);
}

void UILayer::update(float dt)
{
    m_btnPlus->update(dt);
    m_btnMinus->update(dt);
    m_btnReset->update(dt);
}

void UILayer::draw(sf::RenderWindow &window)
{
    float w = window.getSize().x;
    float btnY = 12.f;

    m_btnPlus->setPosition({w - 48.f, btnY});
    m_btnMinus->setPosition({w - 93.f, btnY});
    m_lodText->setPosition({w - 160.f, btnY + 10.f});
    m_btnReset->setPosition({w - 225.f, btnY});

    m_btnReset->draw(window);
    window.draw(*m_lodText);
    m_btnMinus->draw(window);
    m_btnPlus->draw(window);
}

void UILayer::setLodText(int lod)
{
    m_lodText->setString("LOD: " + std::to_string(lod));
}

bool UILayer::isInUIZone(sf::Vector2f pos, float windowWidth)
{
    return pos.x > windowWidth - 260.f && pos.y < 60.f;
}