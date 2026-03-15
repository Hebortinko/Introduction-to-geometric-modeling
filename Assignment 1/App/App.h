#pragma once
#include <SFML/Graphics.hpp>
#include "../HermiteCube/HermiteCube.h"
#include "../UI/UILayer.h"
#include <cmath>

class App
{
private:
  int state;
  static const int LOD_STEP = 1;
  bool m_isDragging = false;

  sf::RenderWindow m_window;
  sf::Vector2f *m_draggedPoint = nullptr;
  sf::Vector2f R0, R1, r0, r1;
  sf::Font m_font;

  HermiteCube m_cube;
  UILayer* m_ui = nullptr;

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
  void updateLogic();
  bool isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos, float radius = 10.0f);
  void onMouseClick(sf::Vector2f pos);
};