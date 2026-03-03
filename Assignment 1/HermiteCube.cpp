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
