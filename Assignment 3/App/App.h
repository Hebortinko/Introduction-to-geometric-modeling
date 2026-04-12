#pragma once

#include "../HermiteSpline/HermiteSpline.h"
#include "../UI/UILayer.h"
#include <SFML/Graphics.hpp>

class App {
private:
  sf::RenderWindow m_window;
  sf::Font m_font;
  HermiteSpline m_spline;
  UILayer *m_ui = nullptr;

  void render();
  void drawScene();
  void resetCanvas();
  void increaseLod();
  void decreaseLod();

public:
  App();
  ~App();
  void run();
  void handleInput(const sf::Event &event);
  bool isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos,
                   float radius = 10.0f);
};
