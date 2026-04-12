#include "App.h"

#include <array>
#include <cmath>
#include <string>
#include <stdexcept>

namespace {
constexpr float DERIVATIVE_VECTOR_DISPLAY_LENGTH = 42.f;

sf::Color getSegmentColor(std::size_t index) {
  static constexpr std::array<sf::Color, 8> palette = {
      sf::Color(239, 83, 80),  sf::Color(66, 165, 245),
      sf::Color(102, 187, 106), sf::Color(255, 202, 40),
      sf::Color(171, 71, 188), sf::Color(255, 112, 67),
      sf::Color(38, 198, 218), sf::Color(141, 110, 99)};

  return palette[index % palette.size()];
}

float vectorLength(const sf::Vector2f &vector) {
  return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f normalize(const sf::Vector2f &vector) {
  const float length = vectorLength(vector);
  if (length == 0.f) {
    return {0.f, 0.f};
  }

  return {vector.x / length, vector.y / length};
}

void drawDerivativeVector(sf::RenderWindow &window, const sf::Vector2f &start,
                          const sf::Vector2f &derivative,
                          const sf::Color &color) {
  if (vectorLength(derivative) < 1e-3f) {
    return;
  }

  const sf::Vector2f displayVector =
      normalize(derivative) * DERIVATIVE_VECTOR_DISPLAY_LENGTH;
  const sf::Vector2f end = start + displayVector;
  sf::VertexArray line(sf::PrimitiveType::Lines, 2);
  line[0].position = start;
  line[0].color = color;
  line[1].position = end;
  line[1].color = color;
  window.draw(line);

  const sf::Vector2f direction = normalize(displayVector);
  const sf::Vector2f perpendicular(-direction.y, direction.x);
  const float headLength = 14.f;
  const float headWidth = 6.f;
  const sf::Vector2f base = end - direction * headLength;

  sf::ConvexShape arrowHead(3);
  arrowHead.setPoint(0, end);
  arrowHead.setPoint(1, base + perpendicular * headWidth);
  arrowHead.setPoint(2, base - perpendicular * headWidth);
  arrowHead.setFillColor(color);
  window.draw(arrowHead);
}
} // namespace

App::App() : m_window(sf::VideoMode({800, 800}), "Hermite spline editor") {
  if (!m_font.openFromFile("assets/Inter/static/Inter_18pt-Regular.ttf")) {
    throw std::runtime_error("Failed to load UI font.");
  }

  Theme::get().font = &m_font;

  m_ui = new UILayer();
  m_ui->init(m_font);

  m_ui->onReset = [this] { resetCanvas(); };
  m_ui->onPlus = [this] { increaseLod(); };
  m_ui->onMinus = [this] { decreaseLod(); };
  m_ui->setLodText(m_spline.getLOD());
}

App::~App() {
  delete m_ui;
}

void App::increaseLod() {
  m_spline.setLOD(m_spline.getLOD() + 1);
  m_ui->setLodText(m_spline.getLOD());
}

void App::decreaseLod() {
  m_spline.setLOD(m_spline.getLOD() - 1);
  m_ui->setLodText(m_spline.getLOD());
}

void App::resetCanvas() {
  m_spline.setToDefaults();
  m_ui->setLodText(m_spline.getLOD());
}

void App::run() {
  sf::Clock clock;
  while (m_window.isOpen()) {
    const float dt = clock.restart().asSeconds();

    while (const std::optional event = m_window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }

      if (event->is<sf::Event::Resized>()) {
        const sf::FloatRect view(
            {0.f, 0.f},
            {static_cast<float>(m_window.getSize().x),
             static_cast<float>(m_window.getSize().y)});
        m_window.setView(sf::View(view));
      }

      handleInput(*event);
      m_ui->handleEvent(*event);
    }

    m_ui->update(dt);
    render();
  }
}

void App::render() {
  m_window.clear(sf::Color::White);
  drawScene();
  m_ui->draw(m_window);
  m_window.display();
}

void App::handleInput(const sf::Event &event) {
  if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
    const sf::Vector2f mousePos(static_cast<float>(mouseBtn->position.x),
                                static_cast<float>(mouseBtn->position.y));

    if (mouseBtn->button == sf::Mouse::Button::Left) {
      if (!m_ui->isInUIZone(mousePos, static_cast<float>(m_window.getSize().x),
                            static_cast<float>(m_window.getSize().y))) {
        m_spline.addPoint(mousePos);
      }
    }

    if (mouseBtn->button == sf::Mouse::Button::Right) {
      const auto &points = m_spline.getPoints();
      for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (isMouseOver(mousePos, points[static_cast<std::size_t>(i)],
                        m_spline.getPickedRadius())) {
          m_spline.setDraggedPointIndex(i);
          break;
        }
      }
    }
  }

  if (const auto *mouseMov = event.getIf<sf::Event::MouseMoved>()) {
    const int idx = m_spline.getDraggedPointIndex();
    if (idx != -1) {
      m_spline.setPoint(idx, {static_cast<float>(mouseMov->position.x),
                              static_cast<float>(mouseMov->position.y)});
    }
  }

  if (const auto *mouseRel = event.getIf<sf::Event::MouseButtonReleased>()) {
    if (mouseRel->button == sf::Mouse::Button::Right) {
      m_spline.setDraggedPointIndex(-1);
    }
  }
}

void App::drawScene() {
  const auto &points = m_spline.getPoints();
  if (points.empty()) {
    return;
  }

  const auto derivatives = m_spline.getDerivatives();

  sf::VertexArray polygon(sf::PrimitiveType::LineStrip, points.size());
  for (std::size_t i = 0; i < points.size(); ++i) {
    polygon[i].position = points[i];
    polygon[i].color = sf::Color(100, 100, 100, 150);
  }
  m_window.draw(polygon);

  const auto curveSegments = m_spline.sampleSegments();
  for (std::size_t segmentIndex = 0; segmentIndex < curveSegments.size();
       ++segmentIndex) {
    const auto &curvePoints = curveSegments[segmentIndex];
    if (curvePoints.size() < 2) {
      continue;
    }

    sf::VertexArray curveLine(sf::PrimitiveType::LineStrip, curvePoints.size());
    const sf::Color color = getSegmentColor(segmentIndex);

    for (std::size_t i = 0; i < curvePoints.size(); ++i) {
      curveLine[i].position = curvePoints[i];
      curveLine[i].color = color;
    }

    m_window.draw(curveLine);
  }

  for (std::size_t i = 0; i < points.size() && i < derivatives.size(); ++i) {
    const std::size_t colorIndex =
        (i + 1 < points.size()) ? i : (i > 0 ? i - 1 : 0);
    drawDerivativeVector(m_window, points[i], derivatives[i],
                         getSegmentColor(colorIndex));
  }

  sf::CircleShape circle;
  circle.setPointCount(64);

  for (std::size_t i = 0; i < points.size(); ++i) {
    const bool isEndpoint = (i == 0 || i == points.size() - 1);
    const float radius = isEndpoint ? 10.f : 7.f;

    circle.setRadius(radius);
    circle.setOrigin({radius, radius});
    circle.setFillColor(isEndpoint ? sf::Color::Blue
                                   : sf::Color(220, 80, 80));
    circle.setPosition(points[i]);
    m_window.draw(circle);

    sf::Text label(m_font, std::to_string(i), 16);
    label.setFillColor(sf::Color::Black);
    label.setOutlineColor(sf::Color::White);
    label.setOutlineThickness(2.f);
    label.setPosition(points[i] + sf::Vector2f(12.f, -26.f));
    m_window.draw(label);
  }
}

bool App::isMouseOver(sf::Vector2f mousePos, sf::Vector2f pointPos,
                      float radius) {
  const sf::Vector2f diff = mousePos - pointPos;
  return (diff.x * diff.x + diff.y * diff.y) < (radius * radius);
}
