#include "GLoid.h"
#include "WhatUC.h"
#include "Ball.h"
#include "Functions.h"
#include "Game.h"

using namespace Functions;

Ball::Ball(){
    speed = new point3f(ZERO, ZERO, ZERO);
    nextbounce = new point3f(ZERO, ZERO, ZERO);
    nextspeed = new point3f(ZERO, ZERO, ZERO);
    initspeed = new point3f(initX, initX, 2*initX);
    launchspeed = new point3f(initspeed);
    active = FALSE;
    launched = FALSE;
    rad = base_rad;
    setSize(2*rad, 2*rad, 2*rad);


}

Ball::~Ball(){
    delete speed;
    delete launchspeed;
    delete nextspeed;
    delete initspeed;
    delete nextbounce;
}

void Ball::display(){
    GLUquadricObj* base = NULL;

    if(active)
    {
       base = gluNewQuadric();

       glPushMatrix();
        glTranslatef(place.x, place.y, place.z);
        glPushMatrix();
         glColor3f(0.2f, 0.2f, 0.2f);
         gluSphere(base, rad, 12, 12); ;
        glPopMatrix();
       glPopMatrix();

       gluDeleteQuadric(base);
    }
}

Ball& Ball::animate(double secPerFrame){
    return *this;
}

Ball& Ball::reinit(const point3f_t init){
    if(!active){
          active = TRUE;
    }

    launched = FALSE;
    memset(speed, 0, sizeof(point3f));
    memset(nextbounce, 0, sizeof(point3f));
    launchspeed = launchspeed->deepcopy(init);

    nextspeed = nextspeed->deepcopy(launchspeed);

    return *this;
}

// Launch ball
Ball& Ball::launch()
{
   if(!launched)
   {
      launched = TRUE;

      speed = speed->deepcopy(launchspeed);
      Game::playSound(WAV_LAUNCH);
   }

   return * this;
}
