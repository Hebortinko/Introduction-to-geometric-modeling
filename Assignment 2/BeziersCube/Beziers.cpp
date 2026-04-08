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
}

void Beziers::addPoint(const sf::Vector2f& point){
    controlPoints.push_back(point);
}

const std::vector<sf::Vector2f>& Beziers::getPoints() const{
    return controlPoints;
}