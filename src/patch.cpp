#include "patch.h"
#include "patchcollection.h"
#include "numbergenerator.h"

Patch::Patch(MaterialPointer material) 
  : mMaterial(material),
    mId(0) {
  // Generate patch id
  mId = NumberGenerator::getInstance().generateNextPatchId();
  // Initialize patch with emission light
  mResidualColor = mMaterial->emmissionColor * mMaterial->emissionFactor;
  mAccumulatedColor = mMaterial->emmissionColor;
}

Patch::~Patch() {
}

unsigned int Patch::getId() const {
  return mId;
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
  return getEmissionEnergy().length();
}

void Patch::updateAccumulatedColor(const Color &colorDelta) {
  mAccumulatedColor += colorDelta;
}

void Patch::updateResidualColor(const Color &colorDelta) {
  mResidualColor += colorDelta;
}

void Patch::resetResidualColor() {
  mResidualColor = Color(0.0f, 0.0f, 0.0f);
}

bool comparePatchesByEmissionEmergy(const PatchPointer &patch1, const PatchPointer &patch2) {
  if (patch1->getEmissionEnergyValue() < patch2->getEmissionEnergyValue()) {
    return true;
  }
  return false;
}