#include <algorithm>

#include "radiosityengine.h"

/**
* public:
*/
RadiosityEngine::RadiosityEngine()
  : mScene(NULL),
    mScenePatches(NULL),
    mTotalPatchesArea(0.0f) {
}

RadiosityEngine::~RadiosityEngine() {
}

void RadiosityEngine::setScene(ScenePointer scene) {
  mScene = scene;
}

void RadiosityEngine::setImageResolution(int width, int height) {
  mScene->getCamera()->setImageResolution(width, height);
}

void RadiosityEngine::calculateIllumination(int interationsNumber, float patchSize) {
  mScenePatches = mScene->splitIntoPatches(patchSize);
  
  std::cout << "Total patches number: " << mScenePatches->size() << std::endl;
  
  initialize();

  for (int iteration = 1; iteration <= interationsNumber; ++iteration) {
    std::cout << "Processing iteration " << iteration << "..." << std::endl;
    processIteration();
  }
}

void RadiosityEngine::renderScene() {
  mRayTracer.setScenePatches(mScenePatches);
  mRayTracer.setCamera(mScene->getCamera());
  mRayTracer.renderScene();
}

void RadiosityEngine::saveRenderedImageToFile(const QString& filePath) {
  mRayTracer.saveRenderedImageToFile(filePath);
}

/**
* private:
*/
void RadiosityEngine::initialize() {
  Color averageReflectance;

  for each (auto patch in *mScenePatches) {
    float patchArea = patch->getArea();
    mTotalPatchesArea += patchArea;
    averageReflectance += patch->getMaterial()->reflectance * patchArea;
    mAmbientIlluminationValue += patch->getResidualColor() * patchArea;
  }

  averageReflectance /= mTotalPatchesArea;
  mAvarageReflectanceProgressionSum = Color(1.0f / (1.0f - averageReflectance.r), 1.0f / (1.0f - averageReflectance.g), 1.0f / (1.0f - averageReflectance.b));
  mAmbientIlluminationValue *= mAvarageReflectanceProgressionSum / mTotalPatchesArea;

  std::cout << "Total patches area: " << mTotalPatchesArea << std::endl;
  std::cout << "Average reflectance: " << averageReflectance << std::endl;
  std::cout << "Average reflectance progression sum: " << mAvarageReflectanceProgressionSum << std::endl;
  std::cout << "Ambient illumination value: " << mAmbientIlluminationValue << std::endl;
}

void RadiosityEngine::processIteration() {
  PatchCollectionPointer sourcePatches = getSourcePatches();
  // Sort patches in descending order by emission energy
  std::sort(sourcePatches->rbegin(), sourcePatches->rend(), comparePatchesByEmissionEmergy);

  for each (auto sourcePatch in *sourcePatches) {
    shootRadiosity(sourcePatch);
  }
}

void RadiosityEngine::shootRadiosity(PatchPointer sourcePatch) {
  Color energyDelta = sourcePatch->getEmissionEnergy();
  Color patchReflectedRadiosity = sourcePatch->getMaterial()->reflectance * sourcePatch->getResidualColor() * sourcePatch->getArea();

  // Shoot radiosity
  PatchCollectionPointer visiblePatches = getVisiblePatches(sourcePatch);
  for each (auto visiblePatch in *visiblePatches) {
    Color previousAccumulatedColor = visiblePatch->getAccumulatedColor();
    Color previousResidualColor = visiblePatch->getAccumulatedColor();

    Color radiosityDelta = patchReflectedRadiosity * getFormFactor(sourcePatch, visiblePatch) / visiblePatch->getArea();
    visiblePatch->setAccumulatedColor(previousAccumulatedColor + radiosityDelta);
    visiblePatch->setResidualColor(previousResidualColor + radiosityDelta);

    energyDelta -= radiosityDelta * visiblePatch->getArea();
  }

  // Update
  for each (auto patch in *mScenePatches) {
    Color accumulatedColor = patch->getAccumulatedColor();
    Color ambientDelta = patch->getMaterial()->reflectance * mAmbientIlluminationValue;
    patch->setAccumulatedColor(accumulatedColor + ambientDelta);
  }

  sourcePatch->setResidualColor(Color());
  mAmbientIlluminationValue -= energyDelta * (mAvarageReflectanceProgressionSum / mTotalPatchesArea);
}

PatchCollectionPointer RadiosityEngine::getSourcePatches() const {
  PatchCollectionPointer sourcePatches = PatchCollectionPointer(new PatchCollection());
  Color nullColor = Color();

  // Collect patches with not-null residual color (emmission energy)
  for each (auto patch in *mScenePatches) {
    if (patch->getResidualColor() != nullColor) {
      sourcePatches->push_back(patch);
    }
  }

  return sourcePatches;
}

PatchCollectionPointer RadiosityEngine::getVisiblePatches(PatchPointer patch) const {
  // TODO: implement
  return PatchCollectionPointer(new PatchCollection());
}

float RadiosityEngine::getFormFactor(PatchPointer sourcePatch, PatchPointer visiblePatch) const {
  // TODO: implement
  return 0.0f;
}