#ifndef MARKUP_H
#define MARKUP_H

//#include "Object.h"
#include <iostream>
#include <vector>
#include "Vec3.h"
//#include <glm/glm.hpp>
using namespace std;
//using namespace glm;
const int number_of_laps = 3;
class Markup
{
    public:
        Markup();
        //void loadMarkup(float scale);
        //Object *getArrow();
        void touched();
        void changePosition( Vec3 aposition);
        Vec3 getPosition();
        float getRadius();
        //unsigned int getIndex();
    protected:

    private:
        //Object *arrow;
        float scaling;
        float radius;
        Vec3 position;
        unsigned int index;
        vector <Vec3> track;
        unsigned int lap;
};

#endif // MARKUP_H
