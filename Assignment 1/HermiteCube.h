#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

class HermiteCube {
  static const int DEFAULT_LOD = 140;
  int custom_lod;
  std::vector<sf::Vector2f> m_curvePoints;
  

private:
  std::array<float, 4>calculateBasis(float t) const;

  public:
    HermiteCube();

    void setLOD(int lod);
    void setCurvePoints(std::vector<sf::Vector2f> points);


    int getLOD() const;

    void draw(sf::RenderWindow& window);
    void update(sf::Vector2f R0, sf::Vector2f R1, sf::Vector2f r0, sf::Vector2f r1); };
