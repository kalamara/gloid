#include "GLoid.h"

#include "WhatUC.h"
#include "Pill.h"
#include "Brick.h"

#include "Crosshair.h"

Crosshair::Crosshair(class Game &g){
    game = &g;
    setSize(ONE, ONE, ZERO);
    target = {};
}

Crosshair::~Crosshair(){

}

void Crosshair::display(){

    if(launched){ //display target

        cross = game->getTexture("target");
    } else { //display cross

        cross = game->getTexture("crosshairs");
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cross.first);

    glPushMatrix();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(place.x, place.y, place.z);
    glEnable(GL_BLEND);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 cross.second.w,
                 cross.second.h,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 cross.second.pixels);

    switch(axis){
    case AXIS_X:
        glRotatef(3*RIGHT_ANGLE, ZERO, ONE, ZERO);
        break;

    case -AXIS_X:
        glRotatef(RIGHT_ANGLE, ZERO, ONE, ZERO);
        break;

    case AXIS_Y:
        glRotatef(RIGHT_ANGLE, ONE, ZERO, ZERO);
        break;

    case -AXIS_Y:
        glRotatef(3*RIGHT_ANGLE, ONE, ZERO, ZERO);
        break;

    case -AXIS_Z:
        glRotatef(HALF_CIRCLE, ZERO, ZERO, ONE);
        break;

    default: case AXIS_Z:
        break;
    }
    glBegin(GL_QUADS);
    glTexCoord2f(ONE, ZERO);
    glVertex3f(ONE, -ONE, ZERO);

    glTexCoord2f(ONE, ONE);
    glVertex3f(ONE, ONE, ZERO);

    glTexCoord2f(ZERO, ONE);
    glVertex3f(-ONE, ONE, ZERO);

    glTexCoord2f(ZERO, ZERO);
    glVertex3f(-ONE, -ONE, ZERO);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

Crosshair& Crosshair::animate(double secPerFrame){
    return *this;
}

//raycasting algorithm:
Crosshair& Crosshair::update(const Point3f& start, const Point3f& speed){
    target = {};
//find limits for collision to the background
    Point3f lim;
//direction of movement in terms of positive/negative sign
    Point3i dir = speed.signs();
    lim.x = dir.X * SCENE_MAX;
    lim.y = dir.Y * SCENE_MAX;
    lim.z = dir.Z < 0 ? SCENE_MIN - SCENE_MAX : 0.0f;
//find candidate points of collision to background or bricks
    std::map<int, Point3f> candidates; //a candidate per axis
    std::map<int, float> distances; //each candidates' distance from start

//actual axis of movement including information of motion direction
    int kx = dir.X * AXIS_X;
    int ky = dir.Y * AXIS_Y;
    int kz = dir.Z * AXIS_Z;

    candidates[kx] = start.raycast(speed, AXIS_X, abs(lim.x - start.x));
    candidates[ky] = start.raycast(speed, AXIS_Y, abs(lim.y - start.y));
    candidates[kz] = start.raycast(speed, AXIS_Z, abs(lim.z - start.z));

    //on each axis, moving away from the start
    for(int i = 0; i < INTX; i++){//for each layer of bricks-
        //raycast point of collision
        auto collision = start.raycast(speed,
                                      AXIS_X,
                                      abs(FROMBRICK_X(i) - Brick::side/2));
        auto found = game->getBrickAt(collision, AXIS_X);
        if(found){//check if brick is active, otherwise move to next layer
            candidates[kx] = collision;
            break;
        }
    }

    for(int i = 0; i < INTY; i++){//for each layer of bricks
        //raycast point of collision
        auto collision = start.raycast(speed,
                                       AXIS_Y,
                                       abs(FROMBRICK_Y(i) - Brick::side/2));

        auto found = game->getBrickAt(collision, AXIS_Y);
        if(found){//check if brick is active, otherwise move to next layer
            candidates[ky] = collision;
            break;
        }
    }

    for(int i = 0; i < INTZ; i++){//for each layer of bricks
        //raycast point of collision
        auto collision = start.raycast(speed,
                                       AXIS_Z,
                                       abs(FROMBRICK_Z(i) + Brick::depth + 1));

        auto found = game->getBrickAt(collision, AXIS_Z);
        if(found){
            candidates[kz] = collision;
            break;
       }
    }

    distances[kx] = candidates[kx].dist(start);
    distances[ky] = candidates[ky].dist(start);
    distances[kz] = candidates[kz].dist(start);

    //compare all 3 and find the one that's closest to start
    auto closest = std::min_element(begin(distances),
                                    end(distances),
                                    [](const std::pair<int, float>& p1,
                                       const std::pair<int, float>& p2) {

                                        return p1.second < p2.second;
                                    });
//set crosshair place and axis
    axis = closest->first;
    place.deepcopy(candidates[axis]);
}
