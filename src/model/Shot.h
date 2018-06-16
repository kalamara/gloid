#ifndef _SHOT_H
#define _SHOT_H
// LAZER SHOTS
class Shot: public WhatUC<Shot>
{	
    class Play * gameplay;
    GLUquadric * base;
    static constexpr float speedz = 40.0f;
public:
   int score;
   void display(void);
   Shot& animate(double secPerFrame);
   Shot(class Play *game, const class Point3f &where);
   ~Shot();
};
#endif
