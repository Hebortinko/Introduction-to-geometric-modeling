#include "HermiteCube.h"

HermiteCube::HermiteCube(){
  setLOD(DEFAULT_LOD);
}

void HermiteCube::setLOD(int lod){
  if (lod >= DEFAULT_LOD){
    custom_lod = lod;
  }
  else {
    custom_lod = DEFAULT_LOD;
  }
}

int HermiteCube::getLOD() const {
  return custom_lod;
}

std::array<float, 4> HermiteCube::calculateBasis(float t) const {
  float t_2 = t * t;
  return {
    (1 + 2*t) * (1 - t) * (1 - t),
    t*(1-t)*(1 - t),
    t_2*(3 - 2*t),
    t_2 * (t - 1)
  };
}

void HermiteCube::update(sf::Vector2f R0, sf::Vector2f R1, sf::Vector2f r0, sf::Vector2f r1){
  m_curvePoints.clear();  
  
  float step = 1.0f / static_cast<float>(custom_lod - 1);
  for (int i = 0; i < custom_lod; i++){
    float t = i * step;
    auto h = calculateBasis(t);

    sf::Vector2f point = (h[0] * R0) + (h[3] * R1) + (h[1] * r0) + (h[2] * r1);

    m_curvePoints.emplace_back(point);
  }
}

void HermiteCube::draw(sf::RenderWindow& window){
  if (m_curvePoints.empty()) {
    return;
  }

  sf::VertexArray lines(sf::PrimitiveType::LineStrip, m_curvePoints.size());
  for (size_t i = 0; i < m_curvePoints.size(); i++){
    lines[i].position = m_curvePoints[i];
    lines[i].color = sf::Color::Cyan;
  }

  window.draw(lines);
}
