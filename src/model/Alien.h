#ifndef _ALIEN_H
#define _ALIEN_H
// ALIENS
#define ALIEN_COLORS 3

class Point3f;

class Alien: public WhatUC<Alien>{	
    static constexpr float baseRad = 1.0f;
    static constexpr float particleRad = 0.125f;
    static constexpr float rotSpeed = 2*HALF_CIRCLE;
    static constexpr float zSpeed = 5.0f;

    class Game * game;
public:
    int type = 0;
    float roty = ZERO;
    Point3f speed;
    Point3f pal[ALIEN_COLORS] = {
        {ONE, ONE, ONE}, // White
        {ONE, ONE, ZERO}, // Yellow
        {ONE, ZERO, ZERO},  // Red
    };
    //particles explosion[ALIEN_PARTICLES];

    void display(void);
    Alien& animate(double secPerFrame);
    void die(void);

    Alien(Game & g);
    ~Alien();
private:
     GLUquadricObj * base;
     // Draw a solid cube
     static void solidCube(float side);
     static void solidTetrahedron(float side);

};
#endif//_ALIEN_H
