/*!
 *\file raytracer.cpp
 *\brief Contains RayTracer class definition
 */
#include <QColor>

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
  postProcess();
}

void RayTracer::saveRenderedImageToFile(const QString& filePath) {
  mRenderedImage.save(filePath);
}

RayIntersection RayTracer::calculateNearestIntersectionWithPatch(const Ray &ray, const PatchCollectionPointer &patches) {
  RayIntersection nearestIntersection;

  for each (auto patch in patches->getPatches()) {
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
  const int imageWidth = mCamera->getImageWidth();
  const int imageHeight = mCamera->getImageHeight();
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

  return intersection.patch->getColor(intersection);
}

/**
* Due to floating point operations inaccuracy sometimes small pixel-size holes between patches appears.
* This post processing is intended to remove them from the rendered image.
*/
void RayTracer::postProcess() {
  const int imageWidth = mCamera->getImageWidth();
  const int imageHeight = mCamera->getImageHeight();
  const int radius = 2;
  const int blackComponentTreshhold = 10;
  const float smoothingTreshold = 150.0f;

  QImage resultImage = mRenderedImage.copy();  
  QRgb* renderedImageData = reinterpret_cast< QRgb* >(mRenderedImage.bits());
  QRgb* resultImageData = reinterpret_cast< QRgb* >(resultImage.bits());

  for (int y = 0; y < imageHeight; ++y) {
    for (int x = 0; x < imageWidth; ++x) {
      int index = y * imageWidth + x;
            
      Color averageNeighboursColor;
      int neighboursCount = 0;
      for (int i = -radius; i <= radius; ++i) 
      {
        for (int j = -radius; j <= radius; ++j) 
        {
          if (i == 0 && j == 0) {
            continue;
          }
          
          int neighbourX = x + j;
          int neighbourY = y + i;
          if (neighbourX < 0 || neighbourX >= imageWidth || neighbourY < 0 || neighbourY >= imageHeight) 
          {
            continue;
          }

          int neighbourIndex = neighbourY * imageWidth + neighbourX;
          QColor neighbourColor(renderedImageData[neighbourIndex]);
          averageNeighboursColor += Color(neighbourColor.red(), neighbourColor.green(), neighbourColor.blue());
          neighboursCount++;
        }
      }      
      averageNeighboursColor /= (float)neighboursCount;
      
      QColor pixelColor(renderedImageData[index]);            
      Color color = Color(pixelColor.red(), pixelColor.green(), pixelColor.blue());
      Color newColor;

      if (color.r < blackComponentTreshhold && color.g < blackComponentTreshhold && color.b < blackComponentTreshhold) {
        newColor = averageNeighboursColor;
      } else if (color.r < averageNeighboursColor.r && color.g < averageNeighboursColor.g && color.b < averageNeighboursColor.b &&
            averageNeighboursColor.length() - color.length() > smoothingTreshold) {       
        newColor = averageNeighboursColor;
      } else {
        newColor = color;
      }

      resultImageData[index] = qRgba((int)newColor.r, (int)newColor.g, (int)newColor.b, pixelColor.alpha());
    }
  }

  mRenderedImage = resultImage;
}
