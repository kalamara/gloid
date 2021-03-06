#include "GLoid.h"

#include "WhatUC.h"
#include "Particle.h"
#include "Shot.h"
#include "Vaus.h"
#include "Pill.h"

Vaus::Vaus(Game &g){
   game = &g;
   rad = baseRad;
   setSize(2*rad, 2*rad, rad/2);
}
Vaus::~Vaus(){

}

// Make Vaus larger
Vaus& Vaus::enlarge(){
   if(large == false){
      large = true;
      rad = 2*baseRad;
      setSize(2*rad, 2*rad, rad/2);
      game->playSound("enlarge");
   }
   return *this;
}

Vaus& Vaus::reset(){
   active = true;
   large = false;
   armed = false;
   warping = false;
   place.x = ZERO;
   place.y = ZERO;
   rad = baseRad;
   setSize(2*rad, 2*rad, rad/2);

   return *this;
}

void Vaus::display(){
    base = gluNewQuadric();
    gluQuadricDrawStyle(base, GLU_FILL);
    glPushMatrix();
    glTranslatef(place.x, place.y, ZERO);
    glRotatef(45.0f, ZERO, ZERO, ONE);

    Point3f paparia(rad - 0.5f, 0.625f, ZERO);

    glPushMatrix();
    glEnable(GL_BLEND);
    glColor4f(0.3f, 0.3f, 0.3f, 0.3f);//black
    glTranslatef(ZERO, ZERO, ZERO);
    glScalef(ONE, ONE, 0.25f);
    gluSphere(base, rad, 48, 12);
    glDisable(GL_BLEND);
    glPopMatrix();

    for(uint8_t dir = 0; dir < 2; dir++){
        for(uint8_t rot = 0; rot < 2; rot++){
            glPushMatrix();
            glColor3f(1.0f, ZERO, ZERO);

            if(rot){
                glTranslatef(paparia.x * (1 - 2 * dir), paparia.y, ZERO);
                glRotatef(RIGHT_ANGLE, ONE, ZERO, ZERO);
            }else{
                glTranslatef(-paparia.y, paparia.x * (1 - 2 * dir), ZERO);
                glRotatef(RIGHT_ANGLE, ZERO, ONE, ZERO);
            }

            gluCylinder(base, 0.625f, 0.625f, 1.25f, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef((1 - dir) * rad * (1 - 2 * rot),
                         dir * rad * (1 - 2 * rot), ZERO);

            // If the fire button is down, light up the laser canons
            if(fire){
                glColor3f(0.5f, 0.5f, ONE);
            }else{
                glColor3f(ZERO, ZERO, ONE);
            }
            gluSphere(base, 0.3f, 12, 12);
            glPopMatrix();
        }
    }
    glPopMatrix();
    gluDeleteQuadric(base);
}

// Make Vaus follow the mouse
Vaus& Vaus::animate(double secPerFrame){
   prevx = place.x;
   prevy = place.y;
   mousecntl_t m = game->getMouse();
   auto s = game->getScreen().value_or(screen());
   setPlace((m->X * (SCENE_MAX - SCENE_MIN - size.x))/s.W + SCENE_MIN + size.x/2,
            -(m->Y * (SCENE_MAX - SCENE_MIN - size.y))/s.H + SCENE_MAX - size.y/2,
            0.0f);
   fire = m->leftclick;
   int bonusMode = game->bonusMode;
   switch(bonusMode)
   {
      // If the portal is open and we touch it, warp to the next level
      case B:
         if((place.x >= SCENE_MAX - (size.x/2) - 1)
         && (place.y <= SCENE_MIN + (size.y/2)+5.0f)){
            if(!warping){	
               warping = true;
               game->playSound("warp");//WAV_WARP);
            }
         }
         break;

      case E:
         enlarge();
         break;

      case L:
         armed = true;
         break;

      default:
         break;
   }
   return *this;
}
