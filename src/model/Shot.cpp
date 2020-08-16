#include "GLoid.h"

#include "WhatUC.h"
#include "Pill.h"
#include "Brick.h"
#include "Particle.h"
#include "Alien.h"
#include "Shot.h"
#include "Vaus.h"

Shot::Shot(Game &game, const Point3f &where){
   setSize(0.3f, 0.3f, 1.2f);
   active=true;
   rad = game.vaus->rad;
   setPlace(where.x, where.y, where.z);
   gameplay = &game;
}

Shot::~Shot(){

}

// Display laser shots
void Shot::display(){
    base = gluNewQuadric();
    glPushMatrix();
        glTranslatef(place.x - rad,place.y - rad,place.z);
        glPushMatrix();
            glScalef(ONE, ONE, 4.0f);
            glColor3f(ONE, ONE, ONE);
            gluSphere(base, 0.3f, 12, 12);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(place.x - rad,place.y + rad,place.z);
        glPushMatrix();
            glScalef(ONE, ONE, 4.0f);
            glColor3f(ONE, ONE, ONE);
            gluSphere(base, 0.3f, 12, 12);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(place.x + rad,place.y - rad,place.z);
        glPushMatrix();
            glScalef(ONE, ONE, 4.0f);
            glColor3f(ONE, ONE, ONE);
            gluSphere(base, 0.3f, 12, 12);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(place.x + rad,place.y + rad,place.z);
        glPushMatrix();
            glScalef(ONE, ONE, 4.0f);
            glColor3f(ONE, ONE, ONE);
            gluSphere(base, 0.3f, 12, 12);
        glPopMatrix();
    glPopMatrix();
    gluDeleteQuadric(base);
}

// Animate laser shots
Shot& Shot::animate(double secPerFrame){

    if(active){
        // Check for collision with brick
        auto brick = gameplay->getBrickAt(place);
        if(brick){
            brick->hit();
        }
        if((place.z <= SCENE_MIN - SCENE_MAX + size.z/2) || (score > 0)){
            active = false;
        }
        // Check for collision with alien
       /* for(i = 0; i < ALIENNUM; i++){
            float AR = res3f(Alien[i].place.x - place.x,
                       Alien[i].place.y - place.y,
                       Alien[i].place.z - place.z);

            if(Alien[i].active && (AR <= (size.z / 2) + 1.0f)){
                active = FALSE;
                Alien[i].die();

                score += SCOREALIEN;
            }
        }*/
        place.z -= speedz * secPerFrame;
    }
    return *this;
}

Shot Shot::getShot( Game * g){
    return Shot(*g, Point3f(0,0,0));
}
