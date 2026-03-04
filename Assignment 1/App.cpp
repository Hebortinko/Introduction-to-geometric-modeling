#include "App.h"
#include <iostream>

App::App() : m_window(sf::VideoMode({800, 600}), "Hermite's cube editor"), state(0), m_draggedPoint(nullptr){
}

void App::run(){

  while (m_window.isOpen()) {
    while (const std::optional event = m_window.pollEvent()) {
      m_window.clear(sf::Color::White);
      if (event->is<sf::Event::Closed>()) m_window.close();
      handleInput(*event);
    }
    updateLogic();
    render();
  }
}

void App::render() {
  m_window.clear(sf::Color::White);
  drawScene();
  m_window.display();
}


void App::handleInput(const sf::Event& event){

  if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mouseBtn->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos = sf::Vector2f(mouseBtn->position.x, mouseBtn->position.y);
      onMouseClick(mousePos);
    }
  }
}

void App::drawScene(){

  if (this->state >= 1) {
    sf::CircleShape circle(10.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(this->R0);
    m_window.draw(circle);
  }

  if (this->state >= 2) {
    sf::CircleShape circle(10.f);
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(this->R1);
    m_window.draw(circle);
  }

  if (this->state >= 3) {
    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = this->R0;
    line[0].color = sf::Color::Red;

    line[1].position = this->r0;
    line[1].color = sf::Color::Red;

    m_window.draw(line);
    };

  if (this->state >= 4) {
    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = this->R1;
    line[0].color = sf::Color::Red;

    line[1].position = this->r1;
    line[1].color = sf::Color::Red;

    m_cube.update(R0, R1, r0, r1);
    m_cube.draw(m_window);
    m_window.draw(line);
  }
}

void App::updateLogic(){
  return;
}

bool isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos, float radius = 10.0f){
  sf::Vector2f diff = mousePos - pointPos;
  return (diff.x * diff.x + diff.y * diff.y) < (radius * radius);
}

void App::onMouseClick(sf::Vector2f pos) {
  switch (state) {
    case 0:
      this->R0 = pos;
      this->state = 1;
      break;
    case 1:
      this->R1 = pos;
      this->state = 2;
      break;
    case 2:
      this->r0 = pos;
      this->state = 3;
      break;
    case 3:
      this->r1 = pos;
      this->state = 4;
      break;
  }
}
