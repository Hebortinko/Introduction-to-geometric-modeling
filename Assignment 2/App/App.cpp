#include "App.h"
#include <iostream>
#include <algorithm>

App::App()
    : m_window(sf::VideoMode({800, 800}), "Bezier Cube editor"),
      state(0)
{
  if (!m_font.openFromFile("/System/Library/Fonts/Helvetica.ttc"))
    return;

  Theme::get().font = &m_font;

  m_ui = new UILayer();
  m_ui->init(m_font);

  m_ui->onReset = [this] { resetCanvas(); };
  m_ui->onPlus  = [this] { increaseLod(); };
  m_ui->onMinus = [this] { decreaseLod(); };
  m_ui->setLodText(bezier.getLOD());
  
}

App::~App()
{
  delete m_ui;
}

void App::increaseLod()
{
    bezier.setLOD(bezier.getLOD() + 1);
    m_ui->setLodText(bezier.getLOD());
}

void App::decreaseLod()
{
  bezier.setLOD(bezier.getLOD() - 1);
  m_ui->setLodText(bezier.getLOD());
}

void App::resetCanvas()
{
    bezier.setToDefaults();
    m_ui->setLodText(bezier.getLOD());
    m_ui->setSliderValue(bezier.getT());
    
}

void App::run()
{
  sf::Clock clock;
  while (m_window.isOpen())
  {
    float dt = clock.restart().asSeconds();

    while (const std::optional event = m_window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
        m_window.close();

      if (event->is<sf::Event::Resized>())
      {
        sf::FloatRect view({0.f, 0.f},
            {(float)m_window.getSize().x, (float)m_window.getSize().y});
        m_window.setView(sf::View(view));
      }

      handleInput(*event);
      m_ui->handleEvent(*event);
    }

    m_ui->update(dt);
    render();
  }
}

void App::render()
{
  m_window.clear(sf::Color::White);
  drawScene();
  m_ui->draw(m_window);
  m_window.display();
}

void App::handleInput(const sf::Event &event)
{
  if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()){

    if (mouseBtn->button == sf::Mouse::Button::Left ){
        sf::Vector2f mousePos = sf::Vector2f(mouseBtn->position.x, mouseBtn->position.y);

      if (!m_ui->isInUIZone(mousePos, m_window.getSize().x, m_window.getSize().y)){
            bezier.addPoint(mousePos);
      }
    
    }

    if (mouseBtn->button == sf::Mouse::Button::Right){
        sf::Vector2f mousePos = sf::Vector2f(mouseBtn->position.x, mouseBtn->position.y);

        auto& points = bezier.getPoints();
        int index;
        for (int i = 0; i < points.size(); ++i) {
            if (isMouseOver(mousePos, points[i], bezier.getPickedRadius())) {
                bezier.setDraggedPointIndex(i);
                break; 
            }
        }
    }
    
  }

  if (const auto *mouseMov = event.getIf<sf::Event::MouseMoved>()){
    int idx = bezier.getDraggedPointIndex();
    if (idx != -1) {
        sf::Vector2f mousePos(mouseMov->position.x, mouseMov->position.y);
        bezier.setPoint(idx, mousePos); 
    }
  }

  if (const auto *mouseRel = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseRel->button == sf::Mouse::Button::Right) {
            bezier.setDraggedPointIndex(-1);
        }
    }
}

void App::drawScene()
{
    auto& points = bezier.getPoints();
    if (points.empty()) return;

    
    sf::VertexArray lines(sf::PrimitiveType::LineStrip, points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        lines[i].position = points[i];
        lines[i].color = sf::Color(100, 100, 100, 150); 
    }
    m_window.draw(lines);

    std::vector<sf::Vector2f> curvePoints = bezier.sampleCurve();
    if (curvePoints.size() >= 2) {
        sf::VertexArray curveLine(sf::PrimitiveType::LineStrip, curvePoints.size());
        for (size_t i = 0; i < curvePoints.size(); ++i) {
            curveLine[i].position = curvePoints[i];
            curveLine[i].color = sf::Color::Black; 
        }
        m_window.draw(curveLine);
    }

    auto levels = bezier.deCasteljauLevels(m_ui->getSliderValue()); 
    for (size_t l = 1; l < levels.size(); ++l) {
        sf::VertexArray levelLine(sf::PrimitiveType::LineStrip, levels[l].size());
        for (size_t i = 0; i < levels[l].size(); ++i) {
            levelLine[i].position = levels[l][i];
            levelLine[i].color = sf::Color(0, 255, 0, 100); 
        }
        m_window.draw(levelLine);
    
    }

    if (!levels.empty() && !levels.back().empty()) {
        sf::CircleShape pointOnCurve(5.f);
        pointOnCurve.setFillColor(sf::Color::Yellow);
        pointOnCurve.setOrigin({5.f, 5.f});
        pointOnCurve.setPosition(levels.back()[0]); 
        m_window.draw(pointOnCurve);
    }
    
    sf::CircleShape circle(8.f, 64);
    circle.setOrigin({8.f, 8.f});

    for (size_t i = 0; i < points.size(); ++i) {
        if (i == 0 || i == points.size() - 1) {
            circle.setFillColor(sf::Color::Blue); 
            circle.setRadius(10.f); 
        } else {
            circle.setFillColor(sf::Color(220, 80, 80)); 
            circle.setRadius(7.f);
        }
        circle.setPosition(points[i]);
        m_window.draw(circle);
    }


}

bool App::isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos, float radius)
{
  sf::Vector2f diff = mousePos - pointPos;
  return (diff.x * diff.x + diff.y * diff.y) < (radius * radius);
}

