#pragma once
#include <SFML/Graphics.hpp>

#include "HermiteCube.h"

class App {
  private: 
    int state;
    sf::Vector2f* m_draggedPoint;
    sf::Vector2f R0, R1, r0, r1;
    sf::RenderWindow m_window;
    HermiteCube m_cube;

  void render();
  public: 
    App(); 
    void run();
    void drawScene();
    void handleInput(const sf::Event& event);
    void updateLogic();
    bool isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos, float radius = 10.0f);
    void onMouseClick(sf::Vector2f pos);
};
