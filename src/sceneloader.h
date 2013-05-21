/*!
 *\file sceneloader.h
 *\brief Contains SceneLoader class declaration
 */

#pragma once


#include <QDomDocument>

#include "scene.h"
#include "types.h"
#include "shape.h"
#include "quad.h"

class SceneLoader {
  public:
    SceneLoader() {}
    virtual ~SceneLoader() {}

    ScenePointer loadScene(const QString &filePath) const;

  private:
    ScenePointer readScene(const QDomNode &rootNode) const;

    CameraPointer readCamera(const QDomElement &element) const;
    ShapePointer readShape(const QDomElement &element) const;
    MaterialPointer readMaterial(const QDomElement &element) const;

    QuadPointer readQuad(const QDomElement &element, MaterialPointer material) const;

    bool readVector(const QDomElement &element, Vector &vector) const;
    bool readColor(const QDomElement &element, Color &color) const;
    bool readAttributeAsFloat(const QDomElement &element, const QString &attributeName, float &value) const;
    bool readAttributeAsString(const QDomElement &element, const QString &attributeName, QString &value) const;
    bool readChildElementAsVector(const QDomElement &element, const QString &childElementName, Vector &vector) const;
    bool readChildElementAsColor(const QDomElement &element, const QString &childElementName, Color &color) const;
    bool readChildElementAsFloat(const QDomElement &element, const QString &childElementName, const QString &attributeName, float &value) const;
    bool readChildElementAsString(const QDomElement &element, const QString &childElementName, const QString &attributeName, QString &value) const;
};