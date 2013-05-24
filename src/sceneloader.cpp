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
  QString shapeType;
  if (!readAttributeAsString(element, "type", shapeType)) {
    return ShapePointer(NULL);
  }

  MaterialPointer shapeMaterial = readMaterial(element);
  if (shapeMaterial == NULL) {
    return ShapePointer(NULL);
  }

  if (shapeType == "quad") {
    return readQuad(element, shapeMaterial);
  }
  if (shapeType == "box") {
    return readBox(element, shapeMaterial);
  }

  std::cerr << "Scene parsing error: unknown shape type '" << shapeType.toUtf8().constData() << "'" << std::endl;
  return ShapePointer(NULL);
}

QuadPointer SceneLoader::readQuad(const QDomElement &element, MaterialPointer material) const {
  Vector vertex0;
  Vector vertex1;
  Vector vertex2;
  Vector vertex3;

  if (readChildElementAsVector(element, "v0", vertex0) &&
      readChildElementAsVector(element, "v1", vertex1) &&
      readChildElementAsVector(element, "v2", vertex2) &&
      readChildElementAsVector(element, "v3", vertex3)) {
      return QuadPointer(new Quad(vertex0, vertex1, vertex2, vertex3, material));
  }

  return QuadPointer(NULL);
}

BoxPointer SceneLoader::readBox(const QDomElement &element, MaterialPointer material) const {
  Vector center;
  float width;
  float height;
  float depth;
  float rotationXAngle;
  float rotationYAngle;
  float rotationZAngle;

  if (readChildElementAsVector(element, "center", center) &&
      readChildElementAsFloat(element, "width", "value", width) &&
      readChildElementAsFloat(element, "height", "value", height) &&
      readChildElementAsFloat(element, "depth", "value", depth),
      readChildElementAsFloat(element, "rotation_x", "angle", rotationXAngle) &&
      readChildElementAsFloat(element, "rotation_y", "angle", rotationYAngle) &&
      readChildElementAsFloat(element, "rotation_z", "angle", rotationZAngle)) {
    return BoxPointer(new Box(center, width, height, depth, rotationXAngle, rotationYAngle, rotationZAngle, material));
  }

  return BoxPointer(NULL);
}


MaterialPointer SceneLoader::readMaterial(const QDomElement &element) const {
  QDomElement materialElement = element.firstChildElement("material");

  if (materialElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no 'material' child element" << std::endl;
    return MaterialPointer(NULL);
  }

  MaterialPointer material = MaterialPointer(new Material());
  if (readChildElementAsColor(materialElement, "emission_color", material->emmissionColor) &&
      readChildElementAsColor(materialElement, "reflectance_color", material->reflectanceColor) &&
      readChildElementAsFloat(materialElement, "emission_factor", "value", material->emissionFactor) &&
      readChildElementAsFloat(materialElement, "reflectance_factor", "value", material->reflectanceFactor)) {
      return material;
  }

  return MaterialPointer(NULL);
}

bool SceneLoader::readVector(const QDomElement &element, Vector &vector) const {  
  if (readAttributeAsFloat(element, "x", vector.x) && 
    readAttributeAsFloat(element, "y", vector.y) && 
    readAttributeAsFloat(element, "z", vector.z)) {
      return true;
  }

  return false;
}

bool SceneLoader::readColor(const QDomElement &element, Color &color) const {
  if (readAttributeAsFloat(element, "r", color.r) && 
    readAttributeAsFloat(element, "g", color.g) && 
    readAttributeAsFloat(element, "b", color.b)) {
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

bool SceneLoader::readChildElementAsColor(const QDomElement &element, const QString &childElementName, Color &color) const {
  QDomElement childElement = element.firstChildElement(childElementName);

  if (childElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no '" << childElementName.toUtf8().constData() << "' child element" << std::endl;
    return false;
  }

  return readColor(childElement, color);
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