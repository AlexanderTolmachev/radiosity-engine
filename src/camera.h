/*!
 *\file camera.h
 *\brief Contains Camera class declaration
 */

#pragma once

#include <QSharedPointer>

#include "types.h"
#include "ray.h"

class Camera;

typedef QSharedPointer<Camera> CameraPointer;

class Camera {
  public:
    Camera(Vector position, Vector up, Vector lookAt, 
           float fov, float nearPlaneDistance);
    virtual ~Camera();

    Ray emitRay(int x, int y) const;

    void setImageResolution(int width, int height);

    int getImageWidth() const;
    int getImageHeight() const;

  private:
    // Camera position (eye) vector
    Vector mPosition;
    // Up direction vector
    Vector mUp;
    // Look at direction vector
    Vector mLookAt;

    float	mFov;
    float	mNearPlaneDistance;
    float mFocusDistance;
    float	mAspectRatio;

    int	mImageWidth;
    int	mImageHeight;

    // Camera internal coordinate system
    Vector mCameraXAxis;
    Vector mCameraYAxis;
    Vector mCameraZAxis;
};