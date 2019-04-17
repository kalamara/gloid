#include "GLoid.h"

#include "WhatUC.h"
#include "Crosshair.h"

Crosshair::Crosshair(class Game &g){
    game = &g;
    setSize(ONE, ONE, ZERO);
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

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

Crosshair& Crosshair::animate(double secPerFrame){
    return *this;
}

Crosshair& Crosshair::update(const Point3f& start, const Point3f& speed){
//raycasting algorithm:
//find limits for collision to the background
    Point3f lim;
//find candidate points of collision to background or bricks
    Point3f candid[3];
    //on each axis, moving away from the start
        //for each layer of bricks
            //raycast point of collision
            //check if brick is active, otherwise move to next layer
//compare all 3 and find the one that's closest to start
//set crosshair place and axis
}
