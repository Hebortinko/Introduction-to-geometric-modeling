#include "App.h"
#include <iostream>
#include <algorithm>

App::App()
    : m_window(sf::VideoMode({800, 600}), "Hermite's cube editor"),
      state(0),
      m_draggedPoint(nullptr)
{

  m_font.openFromFile("/System/Library/Fonts/Helvetica.ttc");
  Theme::get().font = &m_font;

  m_btnPlus = new Button("+", {710.f, 20.f}, {40.f, 35.f});
  m_btnMinus = new Button("-", {660.f, 20.f}, {40.f, 35.f});
  m_lodText = new sf::Text(m_font);

  m_btnPlus->style(Presets::Success());
  m_btnMinus->style(Presets::Danger());

  m_btnPlus->onClick([this]
                     {
        m_lod += 10;
        m_cube.setLOD(m_lod);
        if (state >= 4)
            m_cube.update(R0, R1, r0 - R0, r1 - R1);
        updateLodText(); });

  m_btnMinus->onClick([this]
                      {
        m_lod = std::max(42, m_lod - 10);
        m_cube.setLOD(m_lod);
        if (state >= 4)
            m_cube.update(R0, R1, r0 - R0, r1 - R1);
        updateLodText(); });

  m_lodText->setCharacterSize(16);
  m_lodText->setFillColor(sf::Color::Black);
  m_lodText->setPosition({560.f, 30.f});
  updateLodText();
}

App::~App()
{
  delete m_btnPlus;
  delete m_btnMinus;
  delete m_lodText;
}

void App::updateLodText()
{
  m_lodText->setString("LOD: " + std::to_string(m_lod));
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
        sf::FloatRect view(
            {0.f, 0.f},
            {(float)m_window.getSize().x,
             (float)m_window.getSize().y});
        m_window.setView(sf::View(view));
        render();
      }

      handleInput(*event);
      m_btnPlus->handleEvent(*event);
      m_btnMinus->handleEvent(*event);
    }

    m_btnPlus->update(dt);
    m_btnMinus->update(dt);

    updateLogic();
    render();
  }
}

void App::render()
{
  m_window.clear(sf::Color::White);
  drawScene();

  
  float w = m_window.getSize().x;
  m_btnPlus->setPosition({w - 50.f, 15.f});
  m_btnMinus->setPosition({w - 95.f, 15.f});
  m_lodText->setPosition({w - 180.f, 25.f});

  m_btnPlus->draw(m_window);
  m_btnMinus->draw(m_window);
  m_window.draw(*m_lodText);
  m_window.display();
}

void App::handleInput(const sf::Event &event)
{
  if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
  {
    if (mouseBtn->button == sf::Mouse::Button::Left)
    {
      sf::Vector2f mousePos = sf::Vector2f(mouseBtn->position.x, mouseBtn->position.y);
      if (state >= 1 && isMouseOver(mousePos, R0))
      {
        m_draggedPoint = &R0;
        m_isDragging = true;
      }
      else if (state >= 2 && isMouseOver(mousePos, R1))
      {
        m_draggedPoint = &R1;
        m_isDragging = true;
      }
      else if (state >= 3 && isMouseOver(mousePos, r0))
      {
        m_draggedPoint = &r0;
        m_isDragging = true;
      }
      else if (state >= 4 && isMouseOver(mousePos, r1))
      {
        m_draggedPoint = &r1;
        m_isDragging = true;
      }
      else
      {
  
        onMouseClick(mousePos);
      }
    }
    else if (mouseBtn->button == sf::Mouse::Button::Right)
    {
    }
  }

  if (const auto *mouseMov = event.getIf<sf::Event::MouseMoved>())
  {
    if (m_isDragging && m_draggedPoint)
    {
      *m_draggedPoint = sf::Vector2f(mouseMov->position.x, mouseMov->position.y);
      if (state >= 4)
        m_cube.update(R0, R1, r0 - R0, r1 - R1);
      else if (state >= 3)
        m_cube.update(R0, R1, r0 - R0, sf::Vector2f(0, 0));
      else if (state >= 2)
        m_cube.update(R0, R1, sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    }
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
  const float handleR = 8.f;

  auto drawCircle = [&](sf::Vector2f pos, float radius, sf::Color color)
  {
    sf::CircleShape circle(radius, 64); 
    circle.setFillColor(color);
    circle.setOrigin({radius, radius}); 
    circle.setPosition(pos);
    m_window.draw(circle);
  };


  auto drawTangent = [&](sf::Vector2f from, sf::Vector2f to, float fromR, float toR)
  {
    sf::Vector2f diff = to - from;
    float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (len == 0)
      return;
    sf::Vector2f dir = diff / len; 


    sf::Vector2f start = from + dir * fromR;
    sf::Vector2f end = to - dir * toR;

    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = start;
    line[0].color = sf::Color(180, 0, 0);
    line[1].position = end;
    line[1].color = sf::Color(180, 0, 0);
    m_window.draw(line);
  };


  if (this->state >= 1)
    drawCircle(this->R0, R, sf::Color(50, 150, 255)); 


  if (this->state >= 2)
  {
    drawCircle(this->R1, R, sf::Color(50, 150, 255));
    m_cube.draw(m_window);
  }


  if (this->state >= 3)
  {
    drawTangent(this->R0, this->r0, R, handleR);
    drawCircle(this->r0, handleR, sf::Color(220, 80, 80)); 
    m_cube.draw(m_window);
  }


  if (this->state >= 4)
  {
    drawTangent(this->R1, this->r1, R, handleR);
    drawCircle(this->r1, handleR, sf::Color(220, 80, 80));
    m_cube.draw(m_window);
  }
}

void App::updateLogic()
{
  return;
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
  case 0:
    this->R0 = pos;
    this->state = 1;
    break;
  case 1:
    this->R1 = pos;
    this->state = 2;
    m_cube.update(this->R0, this->R1, sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    break;
  case 2:
    this->r0 = pos;
    this->state = 3;
    m_cube.update(this->R0, this->R1, this->r0 - this->R0, sf::Vector2f(0, 0));
    break;
  case 3:
    this->r1 = pos;
    this->state = 4;
    m_cube.update(this->R0, this->R1, this->r0 - this->R0, this->r1 - this->R1);
    break;
  }
}
