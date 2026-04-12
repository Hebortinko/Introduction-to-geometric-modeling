#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <vector>

struct HermiteSystem {
  std::vector<std::vector<double>> M;
  std::vector<double> Bx;
  std::vector<double> By;
};

class HermiteMatrixSystem {
private:
  static HermiteSystem initializeSystem(std::size_t n);
  static void
  applyLeftBoundaryCondition(const std::vector<sf::Vector2f> &points,
                             HermiteSystem &system);
  static void applyInternalEquations(const std::vector<sf::Vector2f> &points,
                                     HermiteSystem &system);
  static void
  applyRightBoundaryCondition(const std::vector<sf::Vector2f> &points,
                              HermiteSystem &system);
  static HermiteSystem buildSystem(const std::vector<sf::Vector2f> &points);

public:
  static std::vector<sf::Vector2f>
  solveDerivatives(const std::vector<sf::Vector2f> &points);
};
