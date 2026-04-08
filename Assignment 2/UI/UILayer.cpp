#include "UILayer.h"
#include <iomanip>
#include <sstream>

UILayer::UILayer()
    : slider(0.f, 1.f, 0.05f, {80.f, 730.f}, {640.f, 30.f}, 0.50f)
{
    m_btnPlus  = new Button("+",     {0.f, 0.f}, {40.f, 35.f});
    m_btnMinus = new Button("-",     {0.f, 0.f}, {40.f, 35.f});
    m_btnReset = new Button("Reset", {0.f, 0.f}, {55.f, 35.f});

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
    delete m_sliderText;
}

void UILayer::init(sf::Font& font)
{
    m_lodText = new sf::Text(font);
    m_lodText->setCharacterSize(16);
    m_lodText->setFillColor(sf::Color::Black);

    m_sliderText = new sf::Text(font);
    m_sliderText->setCharacterSize(12);
    m_sliderText->setFillColor(sf::Color(80, 80, 80));

    // zaregistruj callback — text sa aktualizuje pri kazdom pohybe
    slider.onChange([this](float val) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << val;
        m_sliderText->setString("t: " + ss.str());
    });

    // nastav pociatocnu hodnotu textu
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << slider.getValue();
    m_sliderText->setString("t: " + ss.str());
}

void UILayer::handleEvent(const sf::Event &event)
{
    m_btnPlus->handleEvent(event);
    m_btnMinus->handleEvent(event);
    m_btnReset->handleEvent(event);
    slider.handleEvent(event);
}

void UILayer::update(float dt)
{
    m_btnPlus->update(dt);
    m_btnMinus->update(dt);
    m_btnReset->update(dt);
    slider.update(dt);
}

void UILayer::draw(sf::RenderWindow &window)
{
    float w = window.getSize().x;
    float btnY = 12.f;

    m_btnPlus->setPosition({w - 48.f, btnY});
    m_btnMinus->setPosition({w - 93.f, btnY});
    m_lodText->setPosition({w - 160.f, btnY + 10.f});
    m_btnReset->setPosition({w - 225.f, btnY});

    // text tesne nad sliderom
    m_sliderText->setPosition({80.f, 712.f});

    m_btnReset->draw(window);
    window.draw(*m_lodText);
    m_btnMinus->draw(window);
    m_btnPlus->draw(window);
    window.draw(*m_sliderText);
    slider.draw(window);
}

void UILayer::setLodText(int lod)
{
    m_lodText->setString("LOD: " + std::to_string(lod));
}

bool UILayer::isInUIZone(sf::Vector2f pos, float windowWidth, float windowHeight)
{
    return (pos.x > windowWidth - 260.f && pos.y < 60.f) || (pos.y > windowHeight - 90.f);
}

void UILayer::setSliderValue(float newVal){
    slider.setValue(newVal);
}

float UILayer::getSliderValue() const {
    return slider.getValue();
}