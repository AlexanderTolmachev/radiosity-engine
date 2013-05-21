#include "patch.h"

Patch::Patch(MaterialPointer material) 
  : mMaterial(material) {
  mExcidentLight = mMaterial->emmission;
}

Patch::~Patch() {
}

const MaterialPointer& Patch::getMaterial() const {
  return mMaterial;
}

const Color& Patch::getIncidentLight() const {
  return mIncidentLight;
}

const Color& Patch::getExcidentLight() const {
  return mExcidentLight;
}

void Patch::setIncidentLight(const Color &incidentLight) {
  mIncidentLight = incidentLight;
}

void Patch::setExcidentLight(const Color &excidentLight) {
  mExcidentLight = excidentLight;
}
