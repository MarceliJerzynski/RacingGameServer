#include "Car.h"

Car::Car()
{
    markup = new Markup();
    Power = 0.01;
    temporaryPower = 0;
    backPower = 0.05;
    v = 0;
    acceleration = 0;
    goForward = false;
    goBackward = false;
    wheelRotation = 0;

    //markup->loadMarkup(0.2);

    body = new Object();
    chassis=new Object();
    headlit=new Object();
    license=new Object();
    FRW =new Object();
    RLW =new Object();
    RRW =new Object();
    FLW =new Object();
    //dodac pozycję! 
    body->setPosition(vec3(25.0f,0.0f,10.0f));
    chassis->setPosition(vec3(25.0f,0.0f,10.0f));
    headlit->setPosition(vec3(25.0f,0.0f,10.0f));
    license->setPosition(vec3(25.0f,0.0f,10.0f));
    FRW ->setPosition(vec3(25.0f,0.0f,10.0f));
    RLW ->setPosition(vec3(25.0f,0.0f,10.0f));
    RRW ->setPosition(vec3(25.0f,0.0f,10.0f));
    FLW ->setPosition(vec3(25.0f,0.0f,10.0f));

    body->setRotation(0,0,0);
    chassis->setRotation(0,0,0);
    headlit->setRotation(0,0,0);
    license->setRotation(0,0,0);
    FRW ->setRotation(0,-180,0);
    RLW ->setRotation(0,0,0);
    RRW ->setRotation(0,180,0);
    FLW ->setRotation(0,0,0);
}

// void Car::loadFromPath(string pathBody,string pathChassis,string pathHeadlit,string pathLicense,string pathWheel, string texPathBody,string texPathChassis,string texPathHeadlit,string texPathLicense, string texPathWheel, float apower, float abpower,vec3 aposition,
//             float rotX, float rotY, float rotZ, float ascale)
// {
// //Ladowanie poszczegolnych obiektow
// //----------------------------------------------------------------------------------------------------------------------
//     markup = new Markup();

//     body=new Object();
//     body->loadFromPath(pathBody,texPathBody,aposition, rotX,rotY,rotZ,ascale);

//     chassis=new Object();
//     chassis->loadFromPath(pathChassis,texPathChassis,aposition, rotX,rotY,rotZ,ascale);

//     headlit=new Object();
//     headlit->loadFromPath(pathHeadlit,texPathHeadlit,aposition, rotX,rotY,rotZ,ascale);

//     license=new Object();
//     license->loadFromPath(pathLicense,texPathLicense,aposition, rotX,rotY,rotZ,ascale);

//     FRW =new Object();
//     FRW->loadFromPath(pathWheel,texPathWheel, aposition, rotX, rotY - 180,rotZ,ascale);

//     RLW =new Object();
//     RLW->loadFromPath(pathWheel,texPathWheel, aposition, rotX, rotY,rotZ,ascale);

//     RRW =new Object();
//     RRW->loadFromPath(pathWheel,texPathWheel, aposition, rotX, rotY - 180,rotZ,ascale);

//     FLW =new Object();
//     FLW->loadFromPath(pathWheel,texPathWheel, aposition, rotX, rotY,rotZ,ascale);

//----------------------------------------------------------------------------------------------------------------------



//Ustalanie wartosci poczatkowych
//----------------------------------------------------------------------------------------------------------------------
    // Power = apower;
    // temporaryPower = 0;
    // backPower = abpower;
    // v = 0;
    // acceleration = 0;
    // goForward = false;
    // goBackward = false;
    // wheelRotation = rotZ;
//----------------------------------------------------------------------------------------------------------------------
//}

// Object * Car::getBody()
// {
//     return body;
// }

// void Car::setV(float av)
// {
//     v = av;
// }

// float Car::getV()
// {
//     return v;
// }
//potrzebuje getPosition
vec3 Car::getPosition()
{
    return body->getPosition();
}

//potrzeba turn z Object
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
//potrzeba turn z Object
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

//potrzeba turn i getRotationY z Object
void Car::turnWheelLeft()
{
    if ( FLW -> getRotationY() - body -> getRotationY() < max_wheel_angle*3.14f/180.0f)
    {
        FLW -> turn(turn_wheel_angle);
        FRW -> turn(turn_wheel_angle);
    }
}
//potrzeba turn i getRotationY z Object
void Car::turnWheelRight()
{
     if   (FLW -> getRotationY() - body -> getRotationY() >-max_wheel_angle*3.14f/180.0f)
    {
        FLW -> turn(-turn_wheel_angle);
        FRW -> turn(-turn_wheel_angle);
    }
}
//nie potrzeba nic
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

//wymaga getPosition, move, getRotationY, setPosition, rotateX z Object
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
    } else
    if (going == 2 )
    {
        if (temporaryPower > -0.5)
            temporaryPower -= backPower;
        else
            temporaryPower = -0.5;
    } else
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

}
//potrzebuje getRotationY z Object
float Car::getRotation()
{
    return body->getRotationY();
}

// void Car::setRotation(float rotX, float rotY, float rotZ)
// {
//     body->setRotation(rotX, rotY, rotZ);
//     headlit->setRotation(rotX, rotY, rotZ);
//     license->setRotation(rotX, rotY, rotZ);
//     chassis->setRotation(rotX, rotY, rotZ);
//     FLW->setRotation(rotX, rotY, rotZ);
//     FRW->setRotation(rotX, rotY - 180, rotZ);
//     RRW->setRotation(rotX, rotY - 180, rotZ);
//     RLW->setRotation(rotX, rotY, rotZ);
// }
//potrzeba getRotationyY z object
float Car::getWheelRotation()
{
    return FLW ->getRotationY() - body -> getRotationY();
}

// Markup * Car::getMarkup()
// {
//     return markup;
// }
//wymaga getPosition z Object, touched z Markup
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

// void Car::render(mat4 V, mat4 P, ShaderProgram *sp)
// {
//     body->render(V, P, sp);
//     chassis->render(V, P, sp);
//     headlit->render(V, P, sp);
//     license->render(V, P, sp);
//     FLW->render(V, P, sp);
//     FRW->render(V, P, sp);
//     RLW->render(V, P, sp);
//     RRW->render(V, P, sp);
// }

// void Car::AI()
// {
//     float deltax =atan2((markup->getPosition().z- body->getPosition().z ),( markup->getPosition().x-body->getPosition().x))+3.14;
//     float angle=abs(fmod(body->getRotationY()+1.57-6.28*100,6.28));
//     if(temporaryPower>0.3)
//     {move(0);}
//     else
//     {move(1);
//     }
//     if(angle>deltax  && angle-deltax>0.1)
//     {
//         if(angle>5 && deltax<2)
//         {
//             turnRight();
//             turnWheelRight();
//         }
//         else
//         {
//             turnLeft();
//            turnWheelLeft();
//         }
//     }
//     if(angle<deltax && deltax-angle>0.1)
//     {
//         if(deltax>5 && angle<2)
//         {
//             turnLeft();
//             turnWheelLeft();
//         }else
//         {
//              turnRight();
//             turnWheelRight();
//         }
//     }
//     checkpointReached();
// }
