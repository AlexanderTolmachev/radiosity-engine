/*!
 *\file raytracer.h
 *\brief Contains RayTracer class declaration
 */
#pragma once

#include <QString>
#include <QImage>

#include "patch.h"
#include "camera.h"
#include "ray.h"
#include "rayintersection.h"

class RayTracer {
  public:
    RayTracer();
    virtual ~RayTracer();

    void setCamera(const CameraPointer &camera);
    void setScenePatches(const PatchCollectionPointer &patches);
    void renderScene();
    void saveRenderedImageToFile(const QString &filePath);

    static RayIntersection calculateNearestIntersectionWithPatch(const Ray &ray, const PatchCollectionPointer &patches);

  private:
    void render();

    Color traceRay(const Ray &ray);
    //RayIntersection calculateNearestIntersectionWithPatch(const Ray &ray) const;

  private:
    CameraPointer mCamera;
    PatchCollectionPointer mScenePatches;
    QImage mRenderedImage;
};

