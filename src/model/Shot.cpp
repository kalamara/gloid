#include "GLoid.h"

#include "game/Play.h"
#include "WhatUC.h"
#include "Brick.h"
#include "Alien.h"
#include "Shot.h"

Shot::Shot(Play &game, const Point3f &where){
   setSize(0.3f, 0.3f, 1.2f);
   active=true;
   base = gluNewQuadric();
   setPlace(where.x, where.y, where.z);
   gameplay = &game;
}

Shot::~Shot(){
    gluDeleteQuadric(base);
}

// Display laser shots
void Shot::display(){
    glPushMatrix();
    glTranslatef(place.x,place.y,place.z);
    glPushMatrix();
    glScalef(ONE, ONE, 4.0f);
    glColor3f(ONE, ONE, ONE);
    gluSphere(base, 0.3f, 12, 12);
    glPopMatrix();
    glPopMatrix();
}

// Animate laser shots
Shot& Shot::animate(double secPerFrame){
    int i;
    int score = 0;

    if(active){
        // Check for collision with brick
        Brick * brick = gameplay->getBrickAt(place);
        if(brick){
            score = brick->hit();//Brick[rel.X][rel.Y][rel.Z].hit();
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

