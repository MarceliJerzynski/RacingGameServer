#include "Markup.h"

Markup::Markup()
{
    Vec3 point(25.0f, 0.0f, -60.0f);
    track.push_back(point);    point.x = 5; point.y = 0; point.z = -80;
    track.push_back(point);    point.x = -12; point.y = 0; point.z = -78;
    track.push_back(point);    point.x = -25; point.y = 0; point.z = -60;
    track.push_back(point);    point.x = -26; point.y = 0; point.z = 47;
    track.push_back(point);    point.x = -10; point.y = 0; point.z = 80;
    track.push_back(point);    point.x = 25; point.y = 0; point.z = 60;
    track.push_back(point);    point.x = 25; point.y = 0; point.z = -10.0f;
    track.push_back(point);
    position  = track[0];
    index = 0;
    lap = 0;
}

// void Markup::loadMarkup( float scale)
// {
    // track.push_back(vec3(25.0f,0.0f,-60.0f));
    // track.push_back(vec3(5, 0, -80));
    // track.push_back(vec3(-12, 0, -78));
    // track.push_back(vec3(-25, 0, -60));
    // track.push_back(vec3(-26, 0, 47));
    // track.push_back(vec3(-10, 0, 80));
    // track.push_back(vec3(25, 0, 60));
    // track.push_back(vec3(25.0f,0.0f,-10.0f));


    //arrow = new Object();
    //arrow->loadFromPath("models/arrow.obj","img/bricks.png", track[0], -90 ,0,0, scale);
    //position = track[0];
    //index = 0;
    // scaling = scale;
    // radius = 20*scale;
    // lap = 0;
//}

// Object *Markup::getArrow()
// {
//     return arrow;
// }

void Markup::touched()
{
        if (index == track.size()-1)  //koniec wycisgu
        {
            index = 0;
            lap++;
            if (lap == number_of_laps)
            {
                cout<<"THE END"<<endl;
            }
        }
        else
        {
            index++;
        }
        changePosition(track[index]);
}

void Markup::changePosition(Vec3 aposition)
{
    position = aposition;
    //arrow->setM(aposition, -90, 0, 0, scaling);
}

Vec3 Markup::getPosition()
{
    return position;
}

float Markup::getRadius()
{
    return radius;
}

// unsigned int Markup::getIndex()
// {
//     return index;
// }
