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

void RadiosityEngine::renderScene(int interationsNumber, float patchSize) {
  mRenderedImage = QImage(mScene->getCamera()->getImageWidth(), mScene->getCamera()->getImageHeight(), QImage::Format_RGB32);
  render(interationsNumber, patchSize);
}

void RadiosityEngine::saveRenderedImageToFile(const QString& filePath) {
  mRenderedImage.save(filePath);
}

/**
 * private:
 */
void RadiosityEngine::render(int interationsNumber, float patchSize) {

}
