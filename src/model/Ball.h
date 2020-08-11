#ifndef _BALL_H_
#define _BALL_H_
class Point3f;
class Crosshair;

class Ball: public WhatUC<Ball>{
private:
    float rad;
    GLUquadricObj* base;


    static constexpr float initX = 100.0f;
    static constexpr float grey = 0.2f;
    static const int slices = 12;
    static const int stacks = 12;

    class Game * game;

public:
    static constexpr float base_rad = 0.625f;
    bool launched = false;
    int nextaxis;
    Crosshair cross;

    Point3f nextbounce;
    Point3f nextspeed;
    Point3f launchspeed;
    Point3f initspeed;

    Ball(class Game &g);
    ~Ball();

    void display();
    Ball& animate(double secPerFrame);
    Ball& launch();
    Ball& reinit(const Point3f& init);
    /**
     * @brief bounce on a plane
     * @param axis
     * @return ball with updated speed
     */
    Ball& bounce(int axis);

    /**
     * @brief bounce on an abstract model of a sphere that is scaled by (X,Y,Z)
     * @param  sphere center
     * @param  scale (X,Y,Z)
     * @return ball with updated speed
     */
    Ball& bounce(const Point3f place, const Point3f size);

    /**
     * @brief ricochet randomly
     * @return ball with updated speed
     */
    Ball& bounce();

    static Ball getBall(Game * g);

    bool collides(const Point3f pl, Point3f sz);
};

#endif //_BALL_H_
