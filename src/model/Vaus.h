#ifndef _VAUS_H
#define _VAUS_H

class Vaus: public WhatUC<Vaus>{
   static constexpr float  base_rad = 2.5f;
   static constexpr float  particle_rad = 0.5f;
   class Game * game;
public:
   BOOL armed;  // type; on lazer mode left click fires
   BOOL large;
   BOOL warping;
   BOOL fire;

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

};
#endif //_VAUS_H
