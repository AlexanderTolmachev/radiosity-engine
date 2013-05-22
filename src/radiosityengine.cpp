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

  for (int iteration = 0; iteration < interationsNumber; ++iteration) {
    std::cout << "Iteration: " << iteration << std::endl;
    // TODO: Implement iterative radiosity calculation
  }
}

void RadiosityEngine::initialize() {
  Color averageReflectance;

  for each (auto patch in *mScenePatches) {
    float patchArea = patch->getArea();
    mTotalPatchesArea += patchArea;
    averageReflectance += patch->getMaterial()->reflectance * patchArea;
    mAmbientIlluminationValue += patch->getExcidentLight() * patchArea;
  }
  
  averageReflectance /= mTotalPatchesArea;
  mAvarageReflectanceProgressionSum = Color(1.0f / (1.0f - averageReflectance.r), 1.0f / (1.0f - averageReflectance.g), 1.0f / (1.0f - averageReflectance.b));
  mAmbientIlluminationValue *= mAvarageReflectanceProgressionSum / mTotalPatchesArea;

  std::cout << "Total patches area: " << mTotalPatchesArea << std::endl;
  std::cout << "Average reflectance: " << averageReflectance << std::endl;
  std::cout << "Average reflectance progression sum: " << mAvarageReflectanceProgressionSum << std::endl;
  std::cout << "Ambient illumination value: " << mAmbientIlluminationValue << std::endl;
}

void RadiosityEngine::renderScene() {
  mRayTracer.setScenePatches(mScenePatches);
  mRayTracer.setCamera(mScene->getCamera());
  mRayTracer.renderScene();
}

void RadiosityEngine::saveRenderedImageToFile(const QString& filePath) {
  mRayTracer.saveRenderedImageToFile(filePath);
}