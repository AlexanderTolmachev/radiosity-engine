/*!
 *\file camera.cpp
 *\brief Contains Camera class definition
 */

#include <math.h>

#include "camera.h"

#define DEFAULT_IMAGE_WIDTH  640
#define DEFAULT_IMAGE_HEIGHT 480

Camera::Camera(Vector position, Vector up, Vector lookAt, 
               float fov, float nearPlaneDistance) 
  : mPosition(position),
    mUp(up),
    mLookAt(lookAt),
    mFov(fov),
    mNearPlaneDistance(nearPlaneDistance),
    mImageWidth(DEFAULT_IMAGE_WIDTH), 
    mImageHeight(DEFAULT_IMAGE_HEIGHT) {
  // Calculate camera internal coordinate system axes
  mCameraZAxis = mLookAt - mPosition;
  mCameraZAxis.normalize();
  mCameraXAxis = mCameraZAxis.crossProduct(mUp);
  mCameraXAxis.normalize();
  mCameraYAxis = mCameraXAxis.crossProduct(mCameraZAxis);
  mCameraYAxis.normalize();
  // Calculate camera aspect ratio
  mAspectRatio = static_cast<float>(mImageWidth) / mImageHeight;
  // Calculate camera focus distance
  float fovAngleInRadians = mFov * static_cast<float>(M_PI) / 180.0;
  mFocusDistance = 1.0 / tan(fovAngleInRadians / 2.0);
}

Camera::~Camera() {
}

void Camera::setImageResolution(int width, int height) {
  mImageWidth = width;
  mImageHeight = height;
  mAspectRatio = static_cast<float>(mImageWidth) / mImageHeight;
}

int Camera::getImageWidth() const {
  return mImageWidth;
}

int Camera::getImageHeight() const {
  return mImageHeight;
}