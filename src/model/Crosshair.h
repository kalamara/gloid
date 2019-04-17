#ifndef _CROSSHAIR_H_
#define _CROSSHAIR_H_
class Point3f;

class Crosshair: public WhatUC<Crosshair>{
    class Game * game;

public:
    bool launched = false;
    int axis = AXIS_Z;

    Crosshair(class Game &g);
    ~Crosshair();

    void display();
    Crosshair& animate(double secPerFrame);

//    static Crosshair getCrosshair(Game * g);

/**
 * Estimate point and direction of next expected collision
 */
    Crosshair& update(const Point3f& start, const Point3f& speed);
private:
    std::pair<GLuint, SDL_Surface> cross;
};

#endif //_CROSSHAIR_H_
