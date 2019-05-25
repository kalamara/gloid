#ifndef _VAUS_H
#define _VAUS_H
class Shot;
class Particle;

class Vaus: public WhatUC<Vaus>{
   static constexpr float  baseRad = 2.5f;
   static constexpr float  particleRad = 0.5f;

   class Game * game;
public:
   // type; on lazer mode left click fires
   bool armed = false;
   bool large = false;
   bool warping = false;
   bool fire = false;
   float rad;

   float prevx;
   float prevy;

   std::list<Shot> shots;
   std::vector<Particle> particles;
   Vaus(Game &g);
   ~Vaus();

   void display();
   Vaus& animate(double secPerFrame);
   Vaus& enlarge();
   Vaus& reset();
private:
    GLUquadricObj* base;
};
#endif //_VAUS_H
