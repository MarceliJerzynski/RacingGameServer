#include "Object.h"

Object::Object()
{
    radius = 0.4;
}

float Object::getRadius()
{
    return radius;
}


float * Object::getVerts()
{
    return verts;
}

float * Object::getNormals()
{
    return normals;
}

float * Object::getColors()
{

    return colors;
}

unsigned int Object::getVertexCount()
{
    return vertexCount;
}

vec3 Object::getPosition()
{
    return position;

}

void Object::setPosition(vec3 aposition)
{
    setM(aposition, rotationX, rotationY, rotationZ, scaling);
}

void Object::turn(float rot)
{
    setM(position, rotationX, rotationY+rot, rotationZ, scaling);
}

void Object::move(float dc)
{
    float dx = -dc * sin(rotationY*3.14f/180.0f);
    float dz = -dc * cos(rotationY*3.14f/180.0f);
    position.x+=dx;
    position.z+=dz;
    setM(position, rotationX, rotationY, rotationZ, scaling);
}

float Object::getRotationY()
{
    return (rotationY-180)*3.14f/180.0f;
}

void Object::setRotation(float rotX, float rotY, float rotZ)
{
   if (rotX != NULL) rotationX = rotX;
   if (rotY != NULL) rotationY = rotY;
   if (rotZ != NULL) rotationZ = rotZ;
   setM(position, rotationX, rotationY, rotationZ, scaling);
}

void Object:: rotateX(float angle)
{
    rotationX += angle;
   setM(position, rotationX, rotationY,rotationZ, scaling);
}
