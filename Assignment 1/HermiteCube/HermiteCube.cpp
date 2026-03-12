#include "HermiteCube.h"

HermiteCube::HermiteCube()
{
  setLOD(DEFAULT_LOD);
}

void HermiteCube::setLOD(int lod)
{
  if (lod >= DEFAULT_LOD)
  {
    custom_lod = lod;
  }
  else
  {
    custom_lod = DEFAULT_LOD;
  }
}

void HermiteCube::setCurvePoints(std::vector<sf::Vector2f> points)
{
  this->m_curvePoints = points;
}

int HermiteCube::getLOD() const
{
  return custom_lod;
}

int HermiteCube::getDefaultLOD() const
{
  return DEFAULT_LOD;
}

std::array<float, 4> HermiteCube::calculateBasis(float t) const
{
  float t_2 = t * t;
  return {
      (1 + 2 * t) * (1 - t) * (1 - t),
      t * (1 - t) * (1 - t),
      t_2 * (3 - 2 * t),
      t_2 * (t - 1)};
}

void HermiteCube::update(sf::Vector2f R0, sf::Vector2f R1, sf::Vector2f r0, sf::Vector2f r1)
{
  m_curvePoints.clear();

  float step = 1.0f / static_cast<float>(custom_lod - 1);
  for (int i = 0; i < custom_lod; i++)
  {
    float t = i * step;
    auto h = calculateBasis(t);

    sf::Vector2f point = (h[0] * R0) + (h[2] * R1) + (h[1] * r0) + (h[3] * r1);

    m_curvePoints.emplace_back(point);
  }
}

void HermiteCube::draw(sf::RenderWindow &window)
{
  if (m_curvePoints.empty())
  {
    return;
  }

  float thickness = 3.f;

  for (size_t i = 0; i < m_curvePoints.size() - 1; i++)
  {
    sf::Vector2f p1 = m_curvePoints[i];
    sf::Vector2f p2 = m_curvePoints[i + 1];

    sf::Vector2f diff = p2 - p1;
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float angle = std::atan2(diff.y, diff.x) * 180.f / 3.14159f;

    sf::RectangleShape segment({length, thickness});
    segment.setOrigin({0.f, thickness / 2.f});
    segment.setPosition(p1);
    segment.setRotation(sf::degrees(angle));
    segment.setFillColor(sf::Color::Cyan);

    window.draw(segment);
  }

  /*
    sf::VertexArray lines(sf::PrimitiveType::LineStrip, m_curvePoints.size());
    for (size_t i = 0; i < m_curvePoints.size(); i++){
      lines[i].position = m_curvePoints[i];
      lines[i].color = sf::Color::Cyan;
    }
  */
}
