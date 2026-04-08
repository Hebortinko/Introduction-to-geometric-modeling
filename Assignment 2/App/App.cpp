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
  if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
  {
    if (mouseBtn->button == sf::Mouse::Button::Left)
    {
      sf::Vector2f mousePos = sf::Vector2f(mouseBtn->position.x, mouseBtn->position.y);
      bezier.addPoint(mousePos);

    }
  }

  if (const auto *mouseMov = event.getIf<sf::Event::MouseMoved>())
  {
    if (m_isDragging && m_draggedPoint)
      *m_draggedPoint = sf::Vector2f(mouseMov->position.x, mouseMov->position.y);
  }

  if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonReleased>())
  {
    if (mouseBtn->button == sf::Mouse::Button::Left)
    {
      m_isDragging = false;
      m_draggedPoint = nullptr;
    }
  }
}

void App::drawScene()
{
  const float R = 10.f;
  std::vector<sf::Vector2f> points = bezier.getPoints();
  
  /*
  
  
  auto drawCircle = [&](sf::Vector2f pos, float radius, sf::Color color)
  {
    sf::CircleShape circle(radius, 64);
    circle.setFillColor(color);
    circle.setOrigin({radius, radius});
    circle.setPosition(pos);
    m_window.draw(circle);
  };

    drawCircle(r0, handleR, sf::Color(220, 80, 80));
    */

   for(auto & element : points){
        sf::CircleShape circle(R, 64);
        circle.setFillColor(sf::Color(220, 80, 80));
        circle.setOrigin({R, R});
        circle.setPosition(element);
        m_window.draw(circle);
   }
   
}


bool App::isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos, float radius)
{
  sf::Vector2f diff = mousePos - pointPos;
  return (diff.x * diff.x + diff.y * diff.y) < (radius * radius);
}

void App::onMouseClick(sf::Vector2f pos)
{
  switch (state)
  {
  case 0: R0 = pos; state = 1; break;
  case 1: R1 = pos; state = 2; break;
  case 2: r0 = pos; state = 3; break;
  case 3: r1 = pos; state = 4; break;
  }
}   