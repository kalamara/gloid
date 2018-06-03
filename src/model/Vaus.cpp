#include "GLoid.h"

#include "WhatUC.h"
#include "Vaus.h"
#include "Pill.h"

Vaus::Vaus(Game* g){
   game = g;
   rad = baseRad;
   setSize(2*baseRad, 2*baseRad, baseRad/2);
   base = gluNewQuadric();
   //speedx = speedy = ZERO;
}
Vaus::~Vaus(){
    gluDeleteQuadric(base);
}

// Make Vaus larger
Vaus& Vaus::enlarge(){
   if(large == false){
      large = true;
      game->playSound(WAV_ENLARGE);
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

   return *this;
}

void Vaus::display(){
    int rot, dir;
    Point3f paparia;

    gluQuadricDrawStyle(base, GLU_FILL);
    glPushMatrix();
    glTranslatef(place.x,place.y,0.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

    if(large){
        rad =  2*baseRad;
    }else{
        rad = baseRad;
    }
    paparia.x = rad - 0.5f;
    paparia.y = 0.625f;
    paparia.z = 0.0f;

    glPushMatrix();
    glEnable(GL_BLEND);
    glColor4f(0.3f, 0.3f, 0.3f, 0.3f);//black
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 0.25f);
    gluSphere(base, baseRad, 48, 12);
    glDisable(GL_BLEND);
    glPopMatrix();

    for(dir = 0; dir < 2; dir++){
        for(rot = 0; rot < 2; rot++){
            glPushMatrix();
            glColor3f(1.0f, 0.0f, 0.0f);

            if(rot){
                glTranslatef(paparia.x * (1 - 2 * dir), paparia.y, 0.0f);
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            }else{
                glTranslatef(-paparia.y, paparia.x * (1 - 2 * dir), 0.0f);
                glRotatef(90.0f, 0.0f, 0.1f, 0.0f);
            }

            gluCylinder(base, 0.625f, 0.625f, 1.25f, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef((1 - dir) * baseRad * (1 - 2 * rot), dir * baseRad * (1 - 2 * rot), 0.0f);

            // If the fire button is down, light up the laser canons
            if(fire){
                glColor3f(0.5f, 0.5f, 1.0f);
            }else{
                glColor3f(0.0f, 0.0f, 1.0f);
            }
            gluSphere(base, 0.3f, 12, 12);
            glPopMatrix();
        }
    }
    glPopMatrix();
}

// Make Vaus follow the mouse
Vaus& Vaus::animate(double secPerFrame){
   //speedx = (prevx-place.x)/secPerFrame;
   //speedy = (prevy-place.y)/secPerFrame;

   prevx = place.x;
   prevy = place.y;
   mousecntl_t m = game->getMouse();
   screen_t s = game->getScreen();
   setPlace((m->X * (SCENE_MAX - SCENE_MIN - size.x))/s->W + SCENE_MIN + size.x/2,
            (m->Y * (SCENE_MAX - SCENE_MIN - size.y))/s->H + SCENE_MAX - size.y/2,
            0.0f);
            
   int bonusMode = game->bonusMode;
   switch(bonusMode)
   {
      // If the portal is open and we touch it, warp to the next level
      case B:
         if((place.x >= SCENE_MAX - (size.x/2) - 1)
         && (place.y <= SCENE_MIN + (size.y/2)+5.0f)){
            if(!warping){	
               warping = true;
               game->playSound(WAV_WARP);
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
