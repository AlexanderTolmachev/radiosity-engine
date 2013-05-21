/*!
 *\file raytracer.h
 *\brief Contains RayTracer class declaration
 */
#pragma once

#include <QString>
#include <QImage>

#include "patch.h"
#include "camera.h"

class RayTracer {
  public:
    RayTracer();
    virtual ~RayTracer();

    void setCamera(const CameraPointer &camera);
    void setScenePatches(const PatchCollectionPointer &patches);
    void renderScene();
    void saveRenderedImageToFile(const QString &filePath);

  private:
    void render();

    //Color traceRay(const Ray &ray, int currentRecursionDepth, bool isRayReflected,
    //               float environmentDensity, float reflectionIntencity, 
    //               RayIntersection &intersection);

    //float calculateFrenselCoefficient(const Vector &sourceDirection, 
    //                                  float sourceEnvironmentDensity, float targetEnvironmentDensity,
    //                                  const Vector &outNormal, bool &isTotalInternalReflection) const;
    //float calculateFrenselCoefficientByFrnsel(float sourceEnvironmentDensity, float targetEnvironmentDensity,
    //                                          float cosThetaTSquared, float cosThetaS) const;
    //float calculateFrenselCoefficientByShlick(const Vector &sourceDirection, const Vector &outNormal,
    //                                          float sourceEnvironmentDensity, float targetEnvironmentDensity) const;

    //Vector claculateRefractedRayDirection(const Vector &originalRayDirection, const Vector &outNormal,
    //                                      float sourceEnvironmentDensity, float targetEnvironmentDensity,
                                          //bool &isTotalInternalReflection) const;

  private:
    CameraPointer mCamera;
    PatchCollectionPointer mScenePatches;
    QImage mRenderedImage;
};

