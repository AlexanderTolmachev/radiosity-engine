#include "patch.h"

Patch::Patch(MaterialPointer material) 
  : mMaterial(material) {
  // Initialize patch with emission light
  mResidualColor = mMaterial->emmission;
  mAccumulatedColor = mMaterial->emmission;
}

Patch::~Patch() {
}

const MaterialPointer& Patch::getMaterial() const {
  return mMaterial;
}

const Color& Patch::getAccumulatedColor() const {
  return mAccumulatedColor;
}

const Color& Patch::getResidualColor() const {
  return mResidualColor;
}

Color Patch::getEmissionEnergy() const {
  return mResidualColor * getArea();
}

float Patch::getEmissionEnergyValue() const {
  Color energyVector = getEmissionEnergy();
  return energyVector.r + energyVector.g + energyVector.b;
}

void Patch::setAccumulatedColor(const Color &incidentLight) {
  mAccumulatedColor = incidentLight;
}

void Patch::setResidualColor(const Color &excidentLight) {
  mResidualColor = excidentLight;
}

bool comparePatchesByEmissionEmergy(const PatchPointer &patch1, const PatchPointer &patch2) {
  if (patch1->getEmissionEnergyValue() < patch2->getEmissionEnergyValue()) {
    return true;
  }
  return false;
}