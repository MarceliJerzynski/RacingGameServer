#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

//#include "OBJLoader.h"
//#include "shaderprogram.h"
//#include "lodepng.h"
#include <cmath>
#include "glm/glm.hpp"
using namespace std;
using namespace glm;
class Object
{
    public:
        Object();
        float getRadius();

        float * getVerts();
        float * getNormals();
        float * getColors();
        // float getRotationY();
        unsigned int getVertexCount();
        vec3 getPosition();
        void setPosition(vec3 aposition);
        void setRotation(float rotX, float rotY, float rotZ);
        void turn(float rot);
        void move(float dc);
        void setM(vec3 aposition, float rotX, float rotY, float rotZ, float scale);
        void rotateX(float angle);
    protected:

        mat4 M;
        float *verts;
        float *normals;
        float *colors;
        unsigned int vertexCount;
        vec3 position;
        float rotationY;
        float rotationX;
        float rotationZ;
        float scaling;
        float radius;
    private:
};

#endif // OBJECT_H
