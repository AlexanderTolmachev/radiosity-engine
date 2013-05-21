/*!
 *\file scene.h
 *\brief Contains Scene class declaration
 */

#pragma once

#include <QSharedPointer>
#include <vector>

#include "camera.h"
#include "shape.h"

class Scene;

typedef QSharedPointer<Scene> ScenePointer;

class Scene {
  public:
    Scene();
    virtual ~Scene();

    void setCamera(const CameraPointer &camera);
    void addShape(const ShapePointer &shape);
    
    const CameraPointer& getCamera() const;

    PatchCollectionPointer splitIntoPatches(float patchSize) const;

  private:
    CameraPointer mCamera;
    std::vector<ShapePointer> mShapes;
};
