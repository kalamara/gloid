#ifndef _BALL_H_
#define _BALL_H_
class Point3f;

class Ball: public WhatUC<Ball>{
    static constexpr float base_rad = 0.625f;
    static constexpr float initX = 10.0f;
    static constexpr float grey = 0.2f;
    static const int slices = 12;
    static const int stacks = 12;

    Game * game;

public:
    bool launched = false;
    int nextaxis;
    Point3f speed;
    Point3f nextbounce;
    Point3f nextspeed;
    Point3f launchspeed;
    Point3f initspeed;

    Ball(Game * g);
    ~Ball();

    void display();
    Ball& animate(double secPerFrame);
    Ball& launch();
    Ball& reinit(const Point3f& init);
    Ball& bounce(const Point3f& x);
private:
     float rad;
     GLUquadricObj* base;
};

#endif //_BALL_H_
