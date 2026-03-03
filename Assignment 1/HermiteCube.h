#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

class HermiteCube {
  static const int DEFAULT_LOD = 42;
  int custom_lod;
  std::vector<sf::Vector2f> m_curvePoints;
  

private:
  std::array<float, 4>calculateBasis(float t) const;

  public:
    HermiteCube();

    void setLOD(int lod);
    int getLOD() const;
};
