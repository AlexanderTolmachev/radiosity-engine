/*!
 *\file sceneloader.cpp
 *\brief Contains SceneLoader class definition
 */

#include <iostream>
#include <QFile>

#include "sceneloader.h"
#include "camera.h"

/*
* public:
*/
ScenePointer SceneLoader::loadScene(const QString &filePath) const {
  QFile sceneFile(filePath);

  sceneFile.open(QIODevice::ReadOnly);
  if (!sceneFile.isOpen()) {
    std::cerr << "Unable to open file at path '" << filePath.toUtf8().constData() << "'" << std::endl;
    return ScenePointer(NULL);
  }

  QDomDocument document;
  QString errorMessge;
  int errorLine, errorColumn;

  if (!document.setContent(&sceneFile, &errorMessge, &errorLine, &errorColumn)) {
    std::cerr << "XML parsing error at line " << errorLine << ", column " << errorColumn << ": " << errorMessge.toUtf8().constData() << std::endl;
    return ScenePointer(NULL);    
  }

  QDomElement rootElement = document.documentElement();
  ScenePointer scene = readScene(rootElement);
  if (scene == NULL) {
    std::cerr << "Failed scene file parsing, check scene format" << std::endl;
  }

  return scene;
}

/*
* private:
*/
ScenePointer SceneLoader::readScene(const QDomNode &rootNode) const {
  if (rootNode.toElement().tagName() != "scene") {
    std::cerr << "Scene parsing error: invalid root tag name, 'scene' expected" << std::endl;
    return ScenePointer(NULL);
  }

  ScenePointer scene = ScenePointer(new Scene());

  bool isCameraIntialized = false;

  QDomElement element = rootNode.firstChildElement();
  while (!element.isNull()) {
    QString elementTagName = element.tagName();        
    
    if (elementTagName == "camera") {
      if (isCameraIntialized) {
        std::cerr << "Scene parsing error: 'camera' tag occurred twice, only one camera is allowed" << std::endl;
        return ScenePointer(NULL);
      }            
      
      CameraPointer camera = readCamera(element);           
      if (camera == NULL) {
        std::cerr << "Scene parsing error: failed camera parameters reading" << std::endl;
        return ScenePointer(NULL);
      }

      scene->setCamera(camera);
      isCameraIntialized = true;

    } else if (elementTagName == "object") {
      ShapePointer shape = readShape(element);
      if (shape == NULL) {
        std::cerr << "Scene parsing error: failed shape parameters reading" << std::endl;
        return ScenePointer(NULL);
      }
      scene->addShape(shape);     
    } else {
      std::cerr << "Scene parsing error: unknown tag '" << elementTagName.toUtf8().constData() << "'" << std::endl;
      return ScenePointer(NULL);
    }

    element = element.nextSiblingElement();
  }
  
  if (!isCameraIntialized) {
    std::cerr << "Scene parsing error: camera parameters are not specified" << std::endl;
    return ScenePointer(NULL);
  }

  return scene;
}

CameraPointer SceneLoader::readCamera(const QDomElement &element) const {
  Vector position;
  Vector up;
  Vector lookAt;
  float fov = 0.0f;
  float nearPlaneDistance = 0.0f;

  if (readChildElementAsVector(element, "pos", position) && 
    readChildElementAsVector(element, "up", up) &&
    readChildElementAsVector(element, "look_at", lookAt) && 
    readChildElementAsFloat(element, "fov", "angle", fov) &&
    readChildElementAsFloat(element, "dist_to_near_plane", "dist", nearPlaneDistance)) {
      return CameraPointer(new Camera(position, up, lookAt, fov, nearPlaneDistance));
  }

  return CameraPointer(NULL);
}

ShapePointer SceneLoader::readShape(const QDomElement &element) const {
  // TODO implement
  return ShapePointer(NULL);
}

bool SceneLoader::readVector(const QDomElement &element, Vector &vector) const {  
  if (readAttributeAsFloat(element, "x", vector.x) && 
    readAttributeAsFloat(element, "y", vector.y) && 
    readAttributeAsFloat(element, "z", vector.z)) {
      return true;
  }

  return false;
}

bool SceneLoader::readAttributeAsFloat(const QDomElement &element, const QString &attributeName, float &value) const {
  if (!element.hasAttribute(attributeName)) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no attribute with name '" << attributeName.toUtf8().constData() << "'" << std::endl;
    return false;
  }

  bool isConversionOk;
  value = element.attribute(attributeName).toFloat(&isConversionOk);    
  if (!isConversionOk) {
    std::cerr << "Scene parsing error: unable to convert '" << attributeName.toUtf8().constData() << "' attribute of element '" << element.tagName().toUtf8().constData() << "' to float" << std::endl;
    return false;
  }

  return true;
}

bool SceneLoader::readAttributeAsString(const QDomElement &element, const QString &attributeName, QString &value) const {
  if (!element.hasAttribute(attributeName)) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no attribute with name '" << attributeName.toUtf8().constData() << "'" << std::endl;
    return false;
  }

  value = element.attribute(attributeName);
  return true;
}

bool SceneLoader::readChildElementAsVector(const QDomElement &element, const QString &childElementName, Vector &vector) const {
  QDomElement childElement = element.firstChildElement(childElementName);

  if (childElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no '" << childElementName.toUtf8().constData() << "' child element" << std::endl;
    return false;
  }

  return readVector(childElement, vector);
}

bool SceneLoader::readChildElementAsFloat(const QDomElement &element, const QString &childElementName, const QString &attributeName, float &value) const {
  QDomElement childElement = element.firstChildElement(childElementName);

  if (childElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no '" << childElementName.toUtf8().constData() << "' child element" << std::endl;
    return false;
  }

  return readAttributeAsFloat(childElement, attributeName, value);
}


bool SceneLoader::readChildElementAsString(const QDomElement &element, const QString &childElementName, const QString &attributeName, QString &value) const {
  QDomElement childElement = element.firstChildElement(childElementName);

  if (childElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no '" << childElementName.toUtf8().constData() << "' child element" << std::endl;
    return false;
  }

  return readAttributeAsString(childElement, attributeName, value);
}