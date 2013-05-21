#pragma once

#include <QString>
#include <QImage>

#include "scene.h"

class RadiosityEngine {
public:
  RadiosityEngine();
  virtual ~RadiosityEngine();

  void setScene(ScenePointer scene);
  void setImageResolution(int width, int height);
  void renderScene(int interationsNumber, float patchSize);
  void saveRenderedImageToFile(const QString &filePath);

private:
  void render(int interationsNumber, float patchSize);
 
private:
  ScenePointer mScene;
  QImage mRenderedImage;
};