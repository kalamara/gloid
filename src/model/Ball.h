#ifndef _BALL_H_
#define _BALL_H_

class Ball: public WhatUC<Ball>{
    static constexpr float base_rad = 0.625f;
    const point3f initspeed = {10.0f, 10.0f, 20.0f};

public:
    float rad;
    BOOL launched;
    int nextaxis;
    point3f_t speed;
    point3f_t nextbounce;
    point3f_t nextspeed;
    point3f_t launchspeed;

    Ball();
    ~Ball();

    void display();
    Ball& animate(double secPerFrame);

    void launch();
    void reinit(point3f_t x);
    int bounce(point3f_t x);

};

#endif //_BALL_H_
