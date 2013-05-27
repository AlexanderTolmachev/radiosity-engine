#include "vertex.h"
#include "numbergenerator.h"

Vertex::Vertex(const Vector &coordinates) 
  : mId(0),
    mCoordinates(coordinates) {
  mId = NumberGenerator::getInstance().generateNextVertexId();
}

Vertex::~Vertex() {
}

unsigned int Vertex::getId() const {
  return mId;
}

Vector Vertex::getCoordinates() const {
  return mCoordinates;
}

Color Vertex::getColor() const {
  return mColor;
}

void Vertex::setColor(const Color &color) {
  mColor = color;
}
