#ifndef CAR_H
#define CAR_H

#include "Object.h"
#include "Markup.h"

const float max_wheel_angle = 45;
const float turn_angle = 0.75;
const float turn_wheel_angle = turn_angle*3;

class Car
{
    public:
        Car();
        vec3 getPosition();
        float getRotation();
        float getWheelRotation();
        bool checkpointReached();
        void turnLeft();
        void turnRight();
        void turnWheelLeft();
        void turnWheelRight();
        int isMoving(); //1 - do przodu, -1 - do tylu, 0 - stoi
        void move(int going);   //0 - nie klika nic, 1 - trzyma W, 2 - trzyma S

    protected:
        Object *body, *chassis, *headlit, *license;
        Object *FLW,*FRW,*RLW,*RRW;
        float Power;
        float backPower;
        float temporaryPower;
        float v;
        float acceleration;
        bool goForward;
        bool goBackward;
        float wheelRotation;
        Markup *markup;
    private:
};

#endif // CAR_H
