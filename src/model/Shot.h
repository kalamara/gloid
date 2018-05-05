#ifndef _SHOT_H
#define _SHOT_H
// LAZER SHOTS
class Shot: public WhatUC<Shot>
{	
    Game * game;
    GLUquadric * base;
    static constexpr float speedz = 40.0f;
public:
   int score;
   void display(void);
   Shot& animate(double secPerFrame);
   Shot(Game *game, const class Point3f &where);
   ~Shot();
};
#endif
