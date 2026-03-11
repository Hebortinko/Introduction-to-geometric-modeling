#pragma once
#include "WidgetStyle.h"
#include <SFML/Graphics.hpp>

namespace Presets {
  inline ButtonStyle Primary(){
    return {
        .bgColor   = sf::Color(70, 130, 180),
        .bgHover   = sf::Color(100, 160, 210),
        .bgPressed = sf::Color(40, 100, 150),
        .textColor = sf::Color::White
      };
  }

  inline ButtonStyle Danger(){
    return {
        .bgColor = sf::Color(200, 50, 50),
        .bgHover = sf::Color(230, 80, 80),
        .bgPressed = sf::Color(170, 30, 30),
        .textColor = sf::Color::White
      };
  }

  inline ButtonStyle Success(){
    return {
      .bgColor   = sf::Color(50, 180, 80),
      .bgHover   = sf::Color(80, 210, 110),
      .bgPressed = sf::Color(30, 150, 60),
      .textColor = sf::Color::White
    }; 
  }

  inline ButtonStyle Ghost(){
    return {
      .bgColor   = sf::Color::Transparent,
      .bgHover   = sf::Color(0,0,0,30),
      .bgPressed = sf::Color(0,0,0,60),
      .textColor = sf::Color(70,130,180)
    };
  }
}
