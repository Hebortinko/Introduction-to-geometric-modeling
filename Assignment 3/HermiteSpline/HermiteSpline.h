#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <vector>

class HermiteSpline {
private:
  static constexpr int MINIMUM_LOD = 42;
  static constexpr float DEFAULT_T = 0.5f;
  static constexpr float MINIMUM_T = 0.0f;
  static constexpr float MAXIMUM_T = 1.0f;

  std::vector<sf::Vector2f> controlPoints;
  int lod = MINIMUM_LOD;
  float t = DEFAULT_T;
  int draggedPointIndex = -1;
  float pickedRadius = 10.0f;

  sf::Vector2f
  evaluateSegment(std::size_t segmentIndex, float localT,
                  const std::vector<sf::Vector2f> &derivatives) const;

public:
  HermiteSpline() = default;
  ~HermiteSpline() = default;

  void addPoint(const sf::Vector2f &point);
  void setPoint(int index, const sf::Vector2f &point);
  const std::vector<sf::Vector2f> &getPoints() const;

  void clear();

  void setLOD(int newLod);
  int getLOD() const;

  void setT(float newT);
  float getT() const;

  void setToDefaults();

  void setDraggedPointIndex(int index);
  int getDraggedPointIndex() const;

  float getPickedRadius() const;

  std::vector<sf::Vector2f> getDerivatives() const;
  sf::Vector2f evaluate(float param) const;
  std::vector<std::vector<sf::Vector2f>> sampleSegments() const;
  std::vector<sf::Vector2f> sampleCurve() const;
};
