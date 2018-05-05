#ifndef _VAUS_H
#define _VAUS_H

class Vaus: public WhatUC<Vaus>{
   static constexpr float  baseRad = 2.5f;
   static constexpr float  particleRad = 0.5f;

   Game * game;
public:
   // type; on lazer mode left click fires
   bool armed = false;
   bool large = false;
   bool warping = false;
   bool fire = false;
   float rad;
   //float speedx;
   //float speedy;
   float prevx;
   float prevy;
//   particles explosion[VAUS_PARTICLES];
   Vaus(Game *g);
   ~Vaus();

   void display();
   Vaus& animate(double secPerFrame);
   Vaus& enlarge();
   Vaus& reset();
private:
    GLUquadricObj* base;
};
#endif //_VAUS_H
