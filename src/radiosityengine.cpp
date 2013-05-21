#include "radiosityengine.h"


/**
* public:
*/
RadiosityEngine::RadiosityEngine()
  : mScene(NULL) {
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
  PatchCollectionPointer scenePatches = mScene->splitIntoPatches(patchSize);

  for (int iteration = 0; iteration < interationsNumber; ++iteration) {
    std::cout << "Iteration: " << iteration << std::endl;
    // TODO: Implement iterative radiosity calculation
  }

  mRayTracer.setScenePatches(scenePatches);
}

void RadiosityEngine::renderScene() {
  mRayTracer.setCamera(mScene->getCamera());
  mRayTracer.renderScene();
}

void RadiosityEngine::saveRenderedImageToFile(const QString& filePath) {
  mRayTracer.saveRenderedImageToFile(filePath);
}