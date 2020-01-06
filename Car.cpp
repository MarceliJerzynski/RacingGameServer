#include "Car.h"

Car::Car()
{
}



vec3 Car::getPosition()
{
    return body->getPosition();
}

void Car::turnLeft()
{
    body->turn(turn_angle);
    chassis->turn(turn_angle);
    license->turn(turn_angle);
    headlit->turn(turn_angle);

    FLW->turn(turn_angle);
    FRW->turn(turn_angle);
    RLW->turn(turn_angle);
    RRW->turn(turn_angle);
}

void Car::turnRight()
{
    body->turn(-turn_angle);
    chassis->turn(-turn_angle);
    license->turn(-turn_angle);
    headlit->turn(-turn_angle);

    FLW->turn(-turn_angle);
    FRW->turn(-turn_angle);
    RLW->turn(-turn_angle);
    RRW->turn(-turn_angle);
}

void Car::turnWheelLeft()
{
    if ( FLW -> getRotationY() - body -> getRotationY() < max_wheel_angle*3.14f/180.0f)
    {
        FLW -> turn(turn_wheel_angle);
        FRW -> turn(turn_wheel_angle);
    }
}

void Car::turnWheelRight()
{
     if   (FLW -> getRotationY() - body -> getRotationY() >-max_wheel_angle*3.14f/180.0f)
    {
        FLW -> turn(-turn_wheel_angle);
        FRW -> turn(-turn_wheel_angle);
    }
}

int Car::isMoving()
{

//zatrzymanie samochodu
//----------------------------------------------------------------------------------------------------------------------
    if (v < 0.02 && v > -0.02)
    {
        return 0;
    }
    if (v > 0.02)
    {
        return 1;
    } else
    return -1;

//----------------------------------------------------------------------------------------------------------------------
}


void Car::move(int going)
//going = 1 <- gracz naciska W
//going = 2 <- gracz naciska S
//going = 0 <- gracz nie trzyma nic
{
    if ( going == 1 )
    {   if (temporaryPower < 1)
            temporaryPower += Power;
        else
            temporaryPower = 1;
    }


    else
    if (going == 2 )
    {
        if (temporaryPower > -0.5)
            temporaryPower -= backPower;
        else
            temporaryPower = -0.5;
    }


    else
    {
       temporaryPower = 0;
    }


    float resistance = v/3*2;
    if (going == 0)
    {
        resistance = resistance*3;
    }


    acceleration = temporaryPower - resistance;
    v = v + acceleration/60;
    float s = v;

    float x1,x2,y1,y2;
    x1 = body->getPosition().x;
    y1 = body->getPosition().z;
    body->move(s);
    chassis->move(s);
    license->move(s);
    headlit->move(s);

    x2 = body->getPosition().x;
    y2 = body->getPosition().z;

    vec3 aposition;
    aposition.x = 0.786 * cos(-body->getRotationY()) - 1.257*sin(-body->getRotationY()) + body->getPosition().x;
    aposition.y = body->getPosition().y + 0.334;
    aposition.z = 0.786 * sin(-body->getRotationY()) + 1.257*cos(-body->getRotationY()) + body->getPosition().z;

    FLW->setPosition(aposition);

    aposition.x = -0.786 * cos(-body->getRotationY()) - 1.257*sin(-body->getRotationY()) + body->getPosition().x;
    aposition.y = body->getPosition().y + 0.334;
    aposition.z = -0.786 * sin(-body->getRotationY()) + 1.257*cos(-body->getRotationY()) + body->getPosition().z;

    FRW->setPosition(aposition);

    aposition.x = -0.786 * cos(-body->getRotationY()) + 1.353*sin(-body->getRotationY()) + body->getPosition().x;
    aposition.y = body->getPosition().y + 0.334;
    aposition.z = -0.786 * sin(-body->getRotationY()) - 1.353*cos(-body->getRotationY()) + body->getPosition().z;

    RRW->setPosition(aposition);

    aposition.x = 0.786 * cos(-body->getRotationY()) + 1.353*sin(-body->getRotationY()) + body->getPosition().x;
    aposition.y = body->getPosition().y + 0.334;
    aposition.z = 0.786 * sin(-body->getRotationY()) - 1.353*cos(-body->getRotationY()) + body->getPosition().z;

    RLW->setPosition(aposition);


//operacje kr�cenia ko�em
//----------------------------------------------------------------------------------------------------------------------
FLW->rotateX(-v*100);
FRW->rotateX(-v*100);
RLW->rotateX(-v*100);
RRW->rotateX(-v*100);

//----------------------------------------------------------------------------------------------------------------------


//obliczanie prostej
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
}

float Car::getRotation()
{
    return body->getRotationY();
}


float Car::getWheelRotation()
{
    return FLW ->getRotationY() - body -> getRotationY();
}


bool Car::checkpointReached()
{
     if ((body->getPosition().x - markup->getPosition().x)*(body->getPosition().x - markup->getPosition().x)
        + (body->getPosition().z - markup->getPosition().z)*(body->getPosition().z - markup->getPosition().z) <= markup->getRadius()*markup->getRadius()) //znaki w drugim zamienione bo z jest odwrotnie
    {
        markup->touched();
        return true;
    }

    return false;
}

