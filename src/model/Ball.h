#ifndef _BALL_H_
#define _BALL_H_

class Ball: public WhatUC<Ball>{
    static constexpr float base_rad = 0.625f;
    static constexpr float initX = 10.0f;
    static constexpr float grey = 0.2f;
    static const int slices = 12;
    static const int stacks = 12;

public:
    float rad;
    BOOL launched;
    int nextaxis;
    point3f_t speed;
    point3f_t nextbounce;
    point3f_t nextspeed;
    point3f_t launchspeed;
    point3f_t initspeed;

    Ball();
    ~Ball();

    void display();
    Ball& animate(double secPerFrame);
    Ball& launch();
    Ball& reinit(const point3f_t init);
    Ball& bounce(point3f_t x);
};

#endif //_BALL_H_
