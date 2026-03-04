#pragma once
#include <SFML/Graphics.hpp>

class App {
  private: 
     
  public: 
    App(); 
    void run();
    void drawScene(sf::RenderWindow& window);
    void handleInput(const sf::Event& event);
    void updateLogic();
    bool isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos, float radius = 10.0f);
};
