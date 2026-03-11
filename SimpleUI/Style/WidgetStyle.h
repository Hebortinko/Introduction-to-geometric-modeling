#pragma once 
#include <SFML/Graphics.hpp>
#include <optional>

struct ButtonStyle {
  std::optional<sf::Color> bgColor   = std::nullopt;
  std::optional<sf::Color> bgHover   = std::nullopt;
  std::optional<sf::Color> bgPressed = std::nullopt;
  std::optional<sf::Color> textColor = std::nullopt;
};
