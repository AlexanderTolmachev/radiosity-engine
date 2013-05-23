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

  postProcess();
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
    // Fill patches hash
    mScenePatchesHash.insert(patch->getId(), patch);
    // Calculate factors for ambient illumination estimation
    float patchArea = patch->getArea();
    mTotalPatchesArea += patchArea;
    averageReflectance += patch->getMaterial()->reflectance * patchArea;
  }

  averageReflectance /= mTotalPatchesArea;
  mTotalReflectance = Color(1.0f / (1.0f - averageReflectance.r), 1.0f / (1.0f - averageReflectance.g), 1.0f / (1.0f - averageReflectance.b));

  std::cout << "Total patches area: " << mTotalPatchesArea << std::endl;
  std::cout << "Average reflectance: " << averageReflectance << std::endl;
  std::cout << "Total reflectance: " << mTotalReflectance << std::endl;
}

void RadiosityEngine::postProcess() {
  // Compute total unshot radiosity
  Color totalUnshotRadiosity;
  for each (auto patch in *mScenePatches) {
    totalUnshotRadiosity += patch->getResidualColor() * patch->getArea();
  }
  totalUnshotRadiosity /= mTotalPatchesArea;

  // Update patches color with estimated ambient component
  Color ambientRadiosity = totalUnshotRadiosity * mTotalReflectance;
  
  std::cout << "Ambient component: " << ambientRadiosity << std::endl;  
  
  for each (auto patch in *mScenePatches) {
    patch->updateAccumulatedColor(patch->getMaterial()->reflectance * ambientRadiosity);
  }
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
  Color sourcePatchRadiosity = sourcePatch->getResidualColor();
  
  // Shoot radiosity
  PatchesAndFactorsCollectionPointer visiblePatchesWithFormFactors = getVisiblePatchesWithFormFactors(sourcePatch);
  for each (auto visiblePatchWithFormFactor in *visiblePatchesWithFormFactors) {
    PatchPointer visiblePatch = visiblePatchWithFormFactor.first;
    float formFactor = visiblePatchWithFormFactor.second;

    Color radiosityDelta = visiblePatch->getMaterial()->reflectance * sourcePatchRadiosity * formFactor * (sourcePatch->getArea() / visiblePatch->getArea());
    visiblePatch->updateAccumulatedColor(radiosityDelta);
    visiblePatch->updateResidualColor(radiosityDelta);  
  }

  sourcePatch->resetResidualColor();
}

PatchCollectionPointer RadiosityEngine::getSourcePatches() const {
  PatchCollectionPointer sourcePatches = PatchCollectionPointer(new PatchCollection());

  // Collect patches with not-null residual color (emmission energy)
  for each (auto patch in *mScenePatches) {
    if (patch->getEmissionEnergyValue() != 0) {
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
  QHash<unsigned int, int> patchesIdToRayHitsCoutHash;

  Hemisphere hemisphere = sourcePatch->getHemisphere();
  for (int i = 0; i < mSamplePointsNumberPerPatch; ++i) {
    Vector hemispherePoint = hemisphere.getRandomCirclePointProjectedToSurface();
    Vector rayDirection = hemispherePoint - hemisphere.getCenter();
    Vector rayOrigin = sourcePatch->getCenter() + sourcePatch->getNormal() * EPS;

    Ray ray = Ray(rayOrigin, rayDirection);
    RayIntersection intersection = RayTracer::calculateNearestIntersectionWithPatch(ray, mScenePatches);
    
    if (!intersection.rayIntersectsWithPatch) {
      continue;
    }

    unsigned int hitPatchId = intersection.patch->getId();
    if (!patchesIdToRayHitsCoutHash.contains(hitPatchId)) {
      // Patch is hit at the first time
      patchesIdToRayHitsCoutHash.insert(hitPatchId, 1);
    } else {
      // Update hits count
      patchesIdToRayHitsCoutHash[hitPatchId] += 1;
    }
  }

  PatchesAndFactorsCollectionPointer visiblePatchesWithFormFactors = PatchesAndFactorsCollectionPointer(new PatchesAndFactorsCollection());    
  QHash<unsigned int, int>::Iterator it;  
  for (it = patchesIdToRayHitsCoutHash.begin(); it != patchesIdToRayHitsCoutHash.end(); ++it) {
    unsigned int patchId = it.key();
    int hitsCount = it.value();

    PatchPointer patch = mScenePatchesHash.value(patchId);
    float formFactor = ((float) hitsCount) / mSamplePointsNumberPerPatch;
    visiblePatchesWithFormFactors->push_back(std::make_pair(patch, formFactor));
  }

  return visiblePatchesWithFormFactors;
}
