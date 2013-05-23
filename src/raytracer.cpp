/*!
 *\file raytracer.cpp
 *\brief Contains RayTracer class definition
 */

#include "raytracer.h"

#define RGBA(r, g, b, a) ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);

/**
*
* public:
*/
RayTracer::RayTracer() 
  : mCamera(NULL),
    mScenePatches(NULL) {
}

RayTracer::~RayTracer() {
}

void RayTracer::setCamera(const CameraPointer &camera) {
  mCamera = camera;
}

void RayTracer::setScenePatches(const PatchCollectionPointer &patches) {
  mScenePatches = patches;
}

void RayTracer::renderScene() {
  mRenderedImage = QImage(mCamera->getImageWidth(), mCamera->getImageHeight(), QImage::Format_RGB32);
  render();
}

void RayTracer::saveRenderedImageToFile(const QString& filePath) {
  mRenderedImage.save(filePath);
}

RayIntersection RayTracer::calculateNearestIntersectionWithPatch(const Ray &ray, const PatchCollectionPointer &patches) {
  RayIntersection nearestIntersection;

  for each (auto patch in *patches) {
    RayIntersection intersection = patch->intersectWithRay(ray);
    if (!intersection.rayIntersectsWithPatch) {
      continue;
    }

    if (intersection.distanceFromRayOrigin < nearestIntersection.distanceFromRayOrigin) {
      nearestIntersection = intersection;
    }
  }

  return nearestIntersection;
}

/*
* private:
*/
void RayTracer::render() {
  int imageWidth = mCamera->getImageWidth();
  int imageHeight = mCamera->getImageHeight();
  unsigned *renderedImageData = reinterpret_cast< unsigned* >(mRenderedImage.bits());

  for (int y = 0; y < imageHeight; ++y) {
    for (int x = 0; x < imageWidth; ++x) {
      Ray ray = mCamera->emitRay(x, y);
      Color pixelColor = traceRay(ray);
      
      unsigned char redComponent   = static_cast<unsigned char>(std::min<unsigned>(pixelColor.r * 255, 255));
      unsigned char greenComponent = static_cast<unsigned char>(std::min<unsigned>(pixelColor.g * 255, 255)); 
      unsigned char blueComponent  = static_cast<unsigned char>(std::min<unsigned>(pixelColor.b * 255, 255));

      int index = y * imageWidth + x;
      *(renderedImageData + index) = RGBA(redComponent, greenComponent, blueComponent, 255);
    }
  }
}

Color RayTracer::traceRay(const Ray &ray) {
  RayIntersection intersection = calculateNearestIntersectionWithPatch(ray, mScenePatches);  
  if (!intersection.rayIntersectsWithPatch) {
    return Color();
  }

  return intersection.patch->getAccumulatedColor();
}

//RayIntersection RayTracer::calculateNearestIntersectionWithPatch(const Ray &ray) const {
//  RayIntersection nearestIntersection;
//
//  for each (auto patch in *mScenePatches) {
//    RayIntersection intersection = patch->intersectWithRay(ray);
//    if (!intersection.rayIntersectsWithPatch) {
//      continue;
//    }
//
//    if (intersection.distanceFromRayOrigin < nearestIntersection.distanceFromRayOrigin) {
//      nearestIntersection = intersection;
//    }
//  }
//
//  return nearestIntersection;
//}