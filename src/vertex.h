#pragma once

#include <QSharedPointer>

#include "types.h"

class Vertex;

typedef QSharedPointer<Vertex> VertexPointer;

class Vertex {
  public:
    Vertex(const Vector &coordinates);
    virtual ~Vertex();

    unsigned int getId() const;
    Vector getCoordinates() const;
    Color getColor() const;

    void setColor(const Color &color);

  private:
    unsigned int mId;
    Vector mCoordinates;
    Color mColor;
};