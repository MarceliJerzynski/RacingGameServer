#ifndef OBJECT_H
#define OBJECT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtc/matrix_transform.hpp>
#include "Vec3.h"
#include <iostream>
#include <cmath>
//#include <vector>
using namespace std;
//using namespace glm;
class Object
{
    public:
        Object();
        //float getRadius();
        //void loadFromPath(string path, string texturePath, vec3 position, float rotX, float rotY, float rotZ, float scale);
        //mat4 getM();
        //float * getVerts();
        //float * getNormals();
        //float * getColors();
        float getRotationY();
        //unsigned int getVertexCount();
        Vec3 getPosition();
        void setPosition(Vec3 &aposition);
        void setRotation(float rotX, float rotY, float rotZ);
        void turn(float rot);
        void move(float dc);
        //void render(mat4 V, mat4 P, ShaderProgram *sp);
        void setM(Vec3 &aposition, float rotX, float rotY, float rotZ, float scale);
        void rotateX(float angle);
    protected:
        //GLuint readTexture(string path);
        //void sendAttributes(float *verts, float *normals, float *colors, ShaderProgram *sp);
        //void disableAttributes(ShaderProgram *sp);
        //void UniformAllMatrix4(mat4 M,mat4 V, mat4 P, ShaderProgram *sp);
        //void sendTexture(ShaderProgram *sp);
        //bool collision_one_object(Object object);    //returns object where collision detected
        //Object * collision(Object *objects); //returns vector of objects where collision detected

        //mat4 M;
        //float *verts;
        //float *normals;
        //float *colors;
        //unsigned int vertexCount;
        Vec3 position;
        float rotationY;
        float rotationX;
        float rotationZ;
        float scaling;
        //GLuint tex;
        float radius;
    private:
};

#endif // OBJECT_H
