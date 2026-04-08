#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <cmath>

class Beziers
{
private:
    const int MINIMUM_LOD = 42;
    const float DEFAULT_T = 0.5f;
    const float MINIMUM_T = 0.0f;
    const float MAXIMUM_T = 1.0f;
    std::vector<sf::Vector2f> controlPoints;
    int lod;
    float t;
    int draggedPointIndex = -1;
    float pickedRadius = 10.0f;

public:
    Beziers();
    ~Beziers() = default;

    void addPoint(const sf::Vector2f& point);
    void setPoint(int index, const sf::Vector2f& point);
    void clear();

    const std::vector<sf::Vector2f>& getPoints() const;

    void setLOD(int newLod);
    int getLOD() const;

    void setT(float newT);
    float getT() const;

    void setToDefaults();

    void setDraggedPointIndex(int index);
    int getDraggedPointIndex() const; 

    float getPickedRadius() const;

    std::vector<std::vector<sf::Vector2f>> deCasteljauLevels(float param) const;
    sf::Vector2f lerp(const sf::Vector2f& A, const sf::Vector2f& B, float t) const ;
    sf::Vector2f evaluate(float param) const;
    std::vector<sf::Vector2f> sampleCurve() const;
};

