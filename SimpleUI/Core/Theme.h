#pragma once
#include <SFML/Graphics.hpp>

struct Theme
{
    sf::Color primary = sf::Color(70, 130, 180);
    sf::Color primaryHover = sf::Color(100, 160, 210);
    sf::Color primaryPress = sf::Color(40, 100, 150);
    sf::Color background = sf::Color(245, 245, 245);
    sf::Color text = sf::Color(30, 30, 30);
    sf::Color textHover = sf::Color(15, 15, 15);
    sf::Color border = sf::Color(200, 200, 200);
    sf::Color borderFocus = sf::Color(70, 130, 180);

    sf::Font *font = nullptr;
    unsigned int fontSize = 16;

    float borderThickness = 2.f;
    float padding = 10.f;

    static Theme &get()
    {
        static Theme instance;
        return instance;
    }
};
