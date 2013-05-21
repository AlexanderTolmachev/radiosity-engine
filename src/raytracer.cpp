/*!
 *\file raytracer.cpp
 *\brief Contains RayTracer class definition
 */

#include "raytracer.h"
//#include "mathcommons.h"

//#define MAX_TRACER_RECURSION_DEPTH 10
//#define RGBA(r, g, b, a) ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);

////#define CALCULATE_FRENSEL_COEFFICIENT_BY_SHLICK

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

/*
* private:
*/
void RayTracer::render() {

}

//void RayTracer::render() {
//  int imageWidth = mScene->getCamera()->getImageWidth();
//  int imageHeight = mScene->getCamera()->getImageHeight();
//  CameraPointer camera = mScene->getCamera();
//  unsigned *renderedImageData = reinterpret_cast< unsigned* >(mRenderedImage.bits());
//
//  for (int y = 0; y < imageHeight; ++y) {
//    for (int x = 0; x < imageWidth; ++x) {
//      RayIntersection intersection;
//
//      Ray ray = camera->emitRay(x, y);
//      Color pixelColor = traceRay(ray, 
//                                  0,      // initial recursion depth is 0
//                                  false,  // ray emitted from camera is not reflected
//                                  1.0,    // air refraction coefficient
//                                  1.0,    // initial reflection
//                                  intersection);
//      
//      unsigned char redComponent   = static_cast<unsigned char>(std::min<unsigned>(pixelColor.r * 255, 255));
//      unsigned char greenComponent = static_cast<unsigned char>(std::min<unsigned>(pixelColor.g * 255, 255)); 
//      unsigned char blueComponent  = static_cast<unsigned char>(std::min<unsigned>(pixelColor.b * 255, 255));
//
//      int index = y * imageWidth + x;
//      *(renderedImageData + index) = RGBA(redComponent, greenComponent, blueComponent, 255);
//    }
//  }
//}
//
//Color RayTracer::traceRay(const Ray &ray, int currentRecursionDepth, bool isRayReflected,
//                          float environmentDensity, float reflectionIntensity, 
//                          RayIntersection &intersection) {
//  if (currentRecursionDepth > MAX_TRACER_RECURSION_DEPTH) {
//    return Color();
//  }
//
//  intersection = mScene->calculateNearestIntersection(ray);
//  
//  if (!intersection.rayIntersectsWithShape) {
//    if (isRayReflected) {
//      return Color();
//    }
//    return mScene->getBackgroundMaterial()->ambientColor;
//  }
//
//  Vector intersectionPoint = ray.getPointAt(intersection.distanceFromRayOrigin);
//  ShapePointer shape = intersection.shape;
//  MaterialPointer shapeMaterial = shape->getMaterial();
//  Vector normal = intersection.normalAtInresectionPoint;
//
//  Color pixelColor = mScene->calculateIlluminationColor(ray, intersection.distanceFromRayOrigin, normal, shapeMaterial);
//
//  /* Process reflection and refraction */
//
//  Vector rayDirection = ray.getDirection();
//  float viewProjection = rayDirection.dotProduct(normal);
//  // Turn normal to the front side
//  Vector outNormal = normal;
//  if (viewProjection > 0.0) {
//    outNormal = -normal;
//  }
//
//  // Calculate fresnel factor
//  bool isTotalInternalReflection = false;
//  float fresnel = calculateFrenselCoefficient(rayDirection, environmentDensity, shapeMaterial->densityFactor, outNormal, isTotalInternalReflection);
//
//  // Calculate reflection
//  if (shapeMaterial->reflectionFactor > 0.0 && reflectionIntensity > EPS_FOR_REFLECTION_RAYS) {
//    // Reflect ray	
//    Vector reflectedRayDirection = rayDirection - outNormal * 2.0 * rayDirection.dotProduct(outNormal);
//    RayIntersection reflectedRayIntersection;
//    Color reflectedColor = traceRay(Ray(intersectionPoint + reflectedRayDirection * EPS_FOR_REFLECTION_RAYS, reflectedRayDirection), 
//                                     currentRecursionDepth + 1,
//                                     true,
//                                     shapeMaterial->densityFactor,
//                                     reflectionIntensity * shapeMaterial->reflectionFactor,      
//                                     reflectedRayIntersection);
//    reflectedColor *=  reflectionIntensity * shapeMaterial->reflectionFactor * fresnel;
//    pixelColor += componentwiseProduct(reflectedColor, shapeMaterial->diffuseColor);
//  }
//
//  // Calculate refraction
//  if (shapeMaterial->refractionFactor > 0.0) {
//    float shapeDensity	= shapeMaterial->densityFactor;
//    Vector reflectedRayDirection = claculateRefractedRayDirection(rayDirection, outNormal, environmentDensity, shapeDensity, isTotalInternalReflection);
//    if (!isTotalInternalReflection) {
//      RayIntersection	refractedRayIntersection;
//      Color refracted  = traceRay(Ray(intersectionPoint + reflectedRayDirection * EPS_FOR_REFLECTION_RAYS, reflectedRayDirection), 
//                                  currentRecursionDepth + 1,
//                                  true,
//                                  shapeDensity, 
//                                  reflectionIntensity,         
//                                  refractedRayIntersection);
//      // Use Burger-Lambert-Beer law
//      if (refractedRayIntersection.rayIntersectsWithShape) {
//        Color absorbance   = (shapeMaterial->diffuseColor) * 0.15f * (-refractedRayIntersection.distanceFromRayOrigin);
//        Color transparency = Color(expf(absorbance.r), expf(absorbance.g), expf(absorbance.b));
//        pixelColor += componentwiseProduct(refracted,  transparency) * (1.0 - fresnel);
//      }
//    }
//  }
//
//  return pixelColor;
//}
//
//float RayTracer::calculateFrenselCoefficient(const Vector &sourceDirection, 
//                                             float sourceEnvironmentDensity, float targetEnvironmentDensity,
//                                             const Vector &outNormal, bool &isTotalInternalReflection) const {
//	float nue	= sourceEnvironmentDensity / targetEnvironmentDensity;
//	float cosThetaS = -outNormal.dotProduct(sourceDirection);
//	float cosThetaTSquared = 1.0 - nue * nue * (1. - cosThetaS * cosThetaS);
//
//	if (cosThetaTSquared < 0.0) {
//		isTotalInternalReflection = true;
//		return 0.0;
//	}
//  isTotalInternalReflection = false;
//
//  #ifdef CALCULATE_FRENSEL_COEFFICIENT_BY_SHLICK
//    return calculateFrenselCoefficientByShlick(sourceDirection, outNormal, sourceEnvironmentDensity, targetEnvironmentDensity);
//  #else
//    return calculateFrenselCoefficientByFrnsel(sourceEnvironmentDensity, targetEnvironmentDensity, cosThetaTSquared, cosThetaS);
//  #endif
//}
//
//float RayTracer::calculateFrenselCoefficientByFrnsel(float sourceEnvironmentDensity, float targetEnvironmentDensity,
//                                                     float cosThetaTSquared, float cosThetaS) const {
//
//	cosThetaTSquared = sqrtf(cosThetaTSquared);
//
//	// Calculate coefficients for s- and p-polarized light
//	float Rs = (sourceEnvironmentDensity * cosThetaS - targetEnvironmentDensity * cosThetaTSquared) / (sourceEnvironmentDensity * cosThetaS + targetEnvironmentDensity * cosThetaTSquared);
//	Rs *= Rs;
//	float Rp = (sourceEnvironmentDensity * cosThetaTSquared - targetEnvironmentDensity * cosThetaS) / (sourceEnvironmentDensity * cosThetaTSquared + targetEnvironmentDensity * cosThetaS);
//	Rp *= Rp;
//
//	return 0.5 * (Rs + Rp);
//}
//
//float RayTracer::calculateFrenselCoefficientByShlick(const Vector &sourceDirection, const Vector &outNormal,
//                                                     float sourceEnvironmentDensity, float targetEnvironmentDensity) const {
//	float r = ((sourceEnvironmentDensity - targetEnvironmentDensity) * (sourceEnvironmentDensity - targetEnvironmentDensity)) / 
//			      ((sourceEnvironmentDensity + targetEnvironmentDensity) * (sourceEnvironmentDensity + targetEnvironmentDensity));
//	float dirDotNormal = 1 + sourceDirection.dotProduct(outNormal);
//
//	return r + (1 - r) * pow(dirDotNormal, 5);
//}
//
//Vector RayTracer::claculateRefractedRayDirection(const Vector &originalRayDirection, const Vector &outNormal,
//                                                 float sourceEnvironmentDensity, float targetEnvironmentDensity,
//                                                 bool &isTotalInternalReflection) const {
//	float nue				 = sourceEnvironmentDensity / targetEnvironmentDensity;
//	float cosThetaS  = -outNormal.dotProduct(originalRayDirection);
//	float cosThetaTSquared = 1.f - nue * nue * (1.f - cosThetaS * cosThetaS);
//
//	if (cosThetaTSquared < 0.f) {
//		isTotalInternalReflection = true;
//		return Vector();
//	}
//
//  Vector reflectedRayDirection = (originalRayDirection * nue) + outNormal * (nue * cosThetaS - sqrtf(cosThetaTSquared));
//  reflectedRayDirection.normalize();
//	return reflectedRayDirection;
//}