#include "Beziers.h"

Beziers::Beziers(){
    setLOD(MINIMUM_LOD);
    setT(DEFAULT_T);
}

void Beziers::setLOD(int newLod){
    if (newLod >= 42){
        lod = newLod;
    }
}

int Beziers::getLOD() const{
    return lod;
}

void Beziers::setT(float newT){
    if (newT >= MINIMUM_T && newT <= MAXIMUM_T)
    {
        t = newT;
    }
}

float Beziers::getT() const {
    return t;
}

void Beziers::setToDefaults(){
    lod = MINIMUM_LOD;
    t   = DEFAULT_T;
    controlPoints.clear();
}

void Beziers::addPoint(const sf::Vector2f& point){
    controlPoints.push_back(point);
}

void Beziers::setPoint(int index, const sf::Vector2f& point){
    controlPoints[index] = point;
}

const std::vector<sf::Vector2f>& Beziers::getPoints() const{
    return controlPoints;
}

void Beziers::setDraggedPointIndex(int index){
    draggedPointIndex = index;
}

int Beziers::getDraggedPointIndex() const {
    return draggedPointIndex;
}

float Beziers::getPickedRadius() const {
    return pickedRadius;
}

sf::Vector2f Beziers::lerp(const sf::Vector2f& A, const sf::Vector2f& B, float t) const
{
    return A * (1.0f - t) + B * t;
}

std::vector<std::vector<sf::Vector2f>> Beziers::deCasteljauLevels(float param) const
{
    std::vector<std::vector<sf::Vector2f>> levels;

    if (controlPoints.empty()) {
        return levels;
    }

    levels.push_back(controlPoints);
    std::vector<sf::Vector2f> current = controlPoints;

    while (current.size() > 1) {
        std::vector<sf::Vector2f> next;

        for (size_t i = 0; i + 1 < current.size(); i++) {
            sf::Vector2f p = lerp(current[i], current[i + 1], param);
            next.push_back(p);
        }

        levels.push_back(next);
        current = next;
    }

    return levels;
}

sf::Vector2f Beziers::evaluate(float param) const
{
    std::vector<std::vector<sf::Vector2f>> levels = deCasteljauLevels(param);

    if (levels.empty()) {
        return sf::Vector2f(0.f, 0.f);
    }

    return levels.back()[0];
}

std::vector<sf::Vector2f> Beziers::sampleCurve() const
{
    std::vector<sf::Vector2f> curvePoints;

    if (controlPoints.size() < 2 || lod < 2) {
        return curvePoints;
    }

    curvePoints.reserve(lod);

    for (int i = 0; i < lod; i++) {
        float t_param = static_cast<float>(i) / static_cast<float>(lod - 1);
        sf::Vector2f pointOnCurve = evaluate(t_param);
        curvePoints.push_back(pointOnCurve);
    }

    return curvePoints;
}