#include "HermiteSpline.h"

#include "../Math/HermiteMatrixSystem.h"
#include <algorithm>
#include <cmath>

void HermiteSpline::addPoint(const sf::Vector2f &point) {
  controlPoints.push_back(point);
}

void HermiteSpline::setPoint(int index, const sf::Vector2f &point) {
  if (index < 0 || static_cast<std::size_t>(index) >= controlPoints.size()) {
    return;
  }

  controlPoints[static_cast<std::size_t>(index)] = point;
}

const std::vector<sf::Vector2f> &HermiteSpline::getPoints() const {
  return controlPoints;
}

void HermiteSpline::clear() {
  controlPoints.clear();
  draggedPointIndex = -1;
}

void HermiteSpline::setLOD(int newLod) {
  if (newLod >= MINIMUM_LOD) {
    lod = newLod;
  }
}

int HermiteSpline::getLOD() const {
  return lod;
}

void HermiteSpline::setT(float newT) {
  if (newT >= MINIMUM_T && newT <= MAXIMUM_T) {
    t = newT;
  }
}

float HermiteSpline::getT() const {
  return t;
}

void HermiteSpline::setToDefaults() {
  lod = MINIMUM_LOD;
  t = DEFAULT_T;
  clear();
}

void HermiteSpline::setDraggedPointIndex(int index) {
  draggedPointIndex = index;
}

int HermiteSpline::getDraggedPointIndex() const {
  return draggedPointIndex;
}

float HermiteSpline::getPickedRadius() const {
  return pickedRadius;
}

std::vector<sf::Vector2f> HermiteSpline::getDerivatives() const {
  if (controlPoints.size() < 2) {
    return {};
  }

  return HermiteMatrixSystem::solveDerivatives(controlPoints);
}

sf::Vector2f
HermiteSpline::evaluateSegment(std::size_t segmentIndex, float localT,
                               const std::vector<sf::Vector2f> &derivatives) const {
  if (segmentIndex + 1 >= controlPoints.size() ||
      derivatives.size() != controlPoints.size()) {
    return {};
  }

  const float t2 = localT * localT;
  const float t3 = t2 * localT;

  const float h00 = 2.0f * t3 - 3.0f * t2 + 1.0f;
  const float h10 = t3 - 2.0f * t2 + localT;
  const float h01 = -2.0f * t3 + 3.0f * t2;
  const float h11 = t3 - t2;

  const sf::Vector2f &p0 = controlPoints[segmentIndex];
  const sf::Vector2f &p1 = controlPoints[segmentIndex + 1];
  const sf::Vector2f &r0 = derivatives[segmentIndex];
  const sf::Vector2f &r1 = derivatives[segmentIndex + 1];

  return {h00 * p0.x + h10 * r0.x + h01 * p1.x + h11 * r1.x,
          h00 * p0.y + h10 * r0.y + h01 * p1.y + h11 * r1.y};
}

sf::Vector2f HermiteSpline::evaluate(float param) const {
  if (controlPoints.empty()) {
    return {};
  }

  if (controlPoints.size() == 1) {
    return controlPoints.front();
  }

  const std::vector<sf::Vector2f> derivatives = getDerivatives();
  if (derivatives.empty()) {
    return {};
  }

  const float clampedParam = std::clamp(param, MINIMUM_T, MAXIMUM_T);
  const std::size_t segmentCount = controlPoints.size() - 1;

  if (clampedParam >= MAXIMUM_T) {
    return evaluateSegment(segmentCount - 1, 1.0f, derivatives);
  }

  const float scaledParam = clampedParam * static_cast<float>(segmentCount);
  const std::size_t segmentIndex = static_cast<std::size_t>(scaledParam);
  const float localT = scaledParam - static_cast<float>(segmentIndex);

  return evaluateSegment(segmentIndex, localT, derivatives);
}

std::vector<std::vector<sf::Vector2f>> HermiteSpline::sampleSegments() const {
  if (controlPoints.size() < 2 || lod < 2) {
    return {};
  }

  const std::vector<sf::Vector2f> derivatives = getDerivatives();
  if (derivatives.empty()) {
    return {};
  }

  const std::size_t segmentCount = controlPoints.size() - 1;
  std::vector<std::vector<sf::Vector2f>> segments;
  segments.reserve(segmentCount);

  for (std::size_t segmentIndex = 0; segmentIndex < segmentCount; ++segmentIndex) {
    std::vector<sf::Vector2f> segmentPoints;
    segmentPoints.reserve(static_cast<std::size_t>(lod));

    for (int i = 0; i < lod; ++i) {
      const float localT =
          static_cast<float>(i) / static_cast<float>(lod - 1);
      segmentPoints.push_back(
          evaluateSegment(segmentIndex, localT, derivatives));
    }

    segments.push_back(std::move(segmentPoints));
  }

  return segments;
}

std::vector<sf::Vector2f> HermiteSpline::sampleCurve() const {
  std::vector<sf::Vector2f> curvePoints;
  const std::vector<std::vector<sf::Vector2f>> segments = sampleSegments();

  if (segments.empty()) {
    return curvePoints;
  }

  for (std::size_t segmentIndex = 0; segmentIndex < segments.size(); ++segmentIndex) {
    const auto &segmentPoints = segments[segmentIndex];
    for (std::size_t i = 0; i < segmentPoints.size(); ++i) {
      if (segmentIndex > 0 && i == 0) {
        continue;
      }

      curvePoints.push_back(segmentPoints[i]);
    }
  }

  return curvePoints;
}
