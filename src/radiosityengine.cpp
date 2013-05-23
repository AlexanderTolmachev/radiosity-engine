#include <algorithm>

#include "radiosityengine.h"

#define _USE_MATH_DEFINES

/**
* public:
*/
RadiosityEngine::RadiosityEngine()
  : mScene(NULL),
    mScenePatches(NULL),
    mTotalPatchesArea(0.0f),
    mSamplePointsNumberPerPatch(0) {
}

RadiosityEngine::~RadiosityEngine() {
}

void RadiosityEngine::setScene(ScenePointer scene) {
  mScene = scene;
}

void RadiosityEngine::setImageResolution(int width, int height) {
  mScene->getCamera()->setImageResolution(width, height);
}

void RadiosityEngine::calculateIllumination(int interationsNumber, float patchSize, int samplePointsNumberPerPatch) {
  mScenePatches = mScene->splitIntoPatches(patchSize);
  mSamplePointsNumberPerPatch = samplePointsNumberPerPatch;

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
  PatchesAndFactorsCollectionPointer visiblePatchesWithFormFactors = getVisiblePatchesWithFormFactors(sourcePatch);
  for each (auto visiblePatchWithFormFactor in *visiblePatchesWithFormFactors) {
    PatchPointer visiblePatch = visiblePatchWithFormFactor.first;
    float formFactor = visiblePatchWithFormFactor.second;

    Color previousAccumulatedColor = visiblePatch->getAccumulatedColor();
    Color previousResidualColor = visiblePatch->getResidualColor();

    Color radiosityDelta = patchReflectedRadiosity * formFactor / visiblePatch->getArea();
    visiblePatch->setAccumulatedColor(previousAccumulatedColor + radiosityDelta);
    visiblePatch->setResidualColor(previousResidualColor + radiosityDelta * visiblePatch->getMaterial()->reflectance);

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

PatchesAndFactorsCollectionPointer RadiosityEngine::getVisiblePatchesWithFormFactors(const PatchPointer &sourcePatch) {
  if (mPatchToVisiblePatchesAndFormFactorsHash.contains(sourcePatch->getId())) {
    return mPatchToVisiblePatchesAndFormFactorsHash.value(sourcePatch->getId());
  }

  PatchesAndFactorsCollectionPointer patchesAndFormFactors = calculateVisiblePatchesWithFormFactors(sourcePatch);
  mPatchToVisiblePatchesAndFormFactorsHash.insert(sourcePatch->getId(), patchesAndFormFactors);
  return patchesAndFormFactors;
}

PatchesAndFactorsCollectionPointer RadiosityEngine::calculateVisiblePatchesWithFormFactors(const PatchPointer &sourcePatch) {
  PatchesAndFactorsCollectionPointer visiblePatchesWithFormFactors = PatchesAndFactorsCollectionPointer(new PatchesAndFactorsCollection());

  for each (auto patch in *mScenePatches) {
    if (patch->getId() == sourcePatch->getId() || !isPatchVisibleFromSourcePatch(sourcePatch, patch)) {
      continue;
    }

    float formFactor = calculateFormFactor(sourcePatch, patch);
    visiblePatchesWithFormFactors->push_back(std::make_pair(patch, formFactor));
    // TODO: add reverse form factor for optimization?
  }  

  return visiblePatchesWithFormFactors;
}

// Shoot ray from source patch center to tested patch center ans assume that 
// if ray is not reaching tested patch then it's not visible
bool RadiosityEngine::isPatchVisibleFromSourcePatch(const PatchPointer &sourcePatch, const PatchPointer &patch) const {
  Vector rayDirection = patch->getCenter() - sourcePatch->getCenter();
  Ray rayToPatch = Ray(sourcePatch->getCenter() + sourcePatch->getNormal() * EPS, rayDirection);
  RayIntersection intersection = RayTracer::calculateNearestIntersectionWithPatch(rayToPatch, mScenePatches);    

  if (!intersection.rayIntersectsWithPatch) {
    return false;
  }

  PatchPointer intersectsWith = intersection.patch;
  if (intersectsWith->getId() != patch->getId()) {
    return false;
  }

  return true;
}

// Calculate form factor with area-to-area Monte Carlo algorithm
// See Michael F. Cohen and John R. Wallace "Radiosity and Realistic Image Synthesis", section 4.10.1, p 94.
float RadiosityEngine::calculateFormFactor(const PatchPointer &sourcePatch, const PatchPointer &visiblePatch) const {
  float formFactor = 0.0f;
  float visiblePatchArea = visiblePatch->getArea();
  float visiblePatchAreaDivNumPoints = visiblePatchArea / mSamplePointsNumberPerPatch;

  for (int i = 0; i < mSamplePointsNumberPerPatch; ++i) {
    Vector sourcePatchPoint = sourcePatch->getRandomPoint();
    Vector visiblePatchPoint = visiblePatch->getRandomPoint();
    
    Vector directionToVisiblePatch = visiblePatchPoint - sourcePatchPoint;
    float squaredDistance = directionToVisiblePatch.length() * directionToVisiblePatch.length();
    directionToVisiblePatch.normalize();

    float sourcePatchAngleCosine = sourcePatch->getNormal().dotProduct(directionToVisiblePatch);
    float visiblePatchAngleCosine = visiblePatch->getNormal().dotProduct(-directionToVisiblePatch);

    float factorDelta = (sourcePatchAngleCosine * visiblePatchAngleCosine) / (M_PI * squaredDistance + visiblePatchAreaDivNumPoints);
    if (factorDelta > 0.0f) {
      formFactor += factorDelta;
    }
  }

  formFactor *= visiblePatchAreaDivNumPoints;

  return formFactor;
}

// TODO: Improve, not good solution
//uint qHash (const PatchPointer &patch) {
//  return qHash(patch->getId());
//};