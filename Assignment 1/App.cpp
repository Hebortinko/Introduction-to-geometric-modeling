#include "App.h"
#include <iostream>

void App::run(){
  
  std::cout << "App is running" << std::endl;
}

App::App(){
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Hermite's cube editor");

  while(window.isOpen()){
    while(const std::optional event = window.pollEvent()){
      if(event->is<sf::Event::Closed>()) window.close();
       handleInput(*event);    
    }

    updateLogic();
  }

  window.clear(sf::Color(30,30,30));
  drawScene(window);
  window.display();
}


void App::handleInput(const sf::Event& event){
  return;
}

void App::drawScene(sf::RenderWindow& window){
  return;
}

void App::updateLogic(){
  return;
}

bool isMouseOver(sf::Vector2f mousePost, sf::Vector2f pointPos, float radius = 10.0f){
  return false;
}
