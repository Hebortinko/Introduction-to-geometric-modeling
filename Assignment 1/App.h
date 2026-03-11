#pragma once
#include <SFML/Graphics.hpp>
#include "HermiteCube.h"
#include "../SimpleUI/Widgets/Button.h"
#include "../SimpleUI/Style/Presets.h"

class App
{
private:
  int state;
  int m_lod = 42;
  sf::Vector2f *m_draggedPoint;
  sf::Vector2f R0, R1, r0, r1;
  sf::RenderWindow m_window;
  HermiteCube m_cube;
  sf::Font m_font;

  bool m_isDragging = false;

  Button *m_btnPlus = nullptr;
  Button *m_btnMinus = nullptr;
  sf::Text *m_lodText = nullptr;

  void render();
  void drawScene();
  void updateLodText();

public:
  App();
  ~App();
  void run();
  void handleInput(const sf::Event &event);
  void updateLogic();
  bool isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos, float radius = 10.0f);
  void onMouseClick(sf::Vector2f pos);
};
