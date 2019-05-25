#ifndef _BRICK_H
#define _BRICK_H

#define SILVER      {0.31f, 0.31f, 0.31f}
#define GOLD        {0.37f, 0.41f, 0.0f}

typedef enum{
    BRIK_NORMAL,
    BRIK_SILVER,
    BRIK_GOLDEN,
    N_BRIK_TYPES
} BRIK_TYPE;

#define N_VERTICES 24
#define N_FACES 6
#define N_PATHS 18
#define N_FACEPATHS 14
class Pill;

class Brick: public WhatUC<Brick>{
    static constexpr float side = 3.3f;
    static constexpr float depth = 1.6f;
    static constexpr float opacity = 0.7f; //when brick is hit
    static const int duration = 200; //hit effect
    static constexpr float unary = 0.5f;
    static constexpr float big = 0.75f;
    Point3f vertice[N_VERTICES] = {
        // Front Face
        {-unary, -unary,  big},
        {unary, -unary,  big},
        {unary,  unary,  big},
        {-unary,  unary,  big},
        // Back Face
        {-unary, -unary, -big},
        {-unary,  unary, -big},
        {unary,  unary, -big},
        {unary, -unary, -big},
        // Top Face
        {-unary,  big, -unary},
        {-unary,  big,  unary},
        { unary,  big,  unary},
        {unary,  big, -unary},
        // Bottom Face
        {-unary, -big, -unary},
        {unary, -big, -unary},
        {unary, -big,  unary},
        {-unary, -big,  unary},
        // Right face
        {big, -unary, -unary},
        {big,  unary, -unary},
        {big,  unary,  unary},
        {big, -unary,  unary},
        // Left Face
        {-big, -unary, -unary},
        {-big, -unary,  unary},
        {-big,  unary,  unary},
        {-big,  unary, -unary},
    };
    Point3f face[N_FACES] = {
        {ZERO, ZERO, ONE},
        {ZERO, ZERO,-ONE},
        {ZERO, ONE, ZERO},
        {ZERO,-ONE, ZERO},
        {ONE, ZERO, ZERO},
        {-ONE, ZERO, ZERO},
    };
    unsigned char path[N_PATHS]  = {3,0,2,1,18,19,17,16,6,7,5,4,23,20,22,21,3,0};
    unsigned char toppath[N_PATHS + 1] = {17,11,18,10, 2,10,3,9,22, 8,23, 5,6, 8,17,11,10, 8, 9};
    unsigned char botpath[N_PATHS + 1] = {15,12,14,13,16,12,7,4,20,12,21,15,0,14,1 ,14,19,13,16};
    unsigned char facepath[N_FACEPATHS] = {0,4,1,5};
    class Game * game;
public:
    Uint32  hit_effect = 0;   // What happens when the brick is hit
    int     hit_counter = 0;  // How many times has the brick been hit?
    int     type = BRIK_NORMAL;         // Type of brick: normal, silver, gold: 0, 1, 2
    // Colors imported from 3DSMax
    Pill pill;

    Point3f rgb = {0,0,0};
    int  hit();

    Brick(){}

    Brick(class Game &g,
          const Point3f& color,
          const Point3i& coords,
          int t);

    ~Brick(){}

    void display();
    Brick& animate(double secPerFrame);

    static std::optional<Point3f> getElement(std::string line, std::string header);
    static std::optional<Point3i> getPos(std::string line);
    static std::optional<Point3f> getColor(std::string line);
    static std::optional<Brick> getBrick(std::istream & ifs, Game * game);

private:
//https://en.wikipedia.org/wiki/Rhombicuboctahedron
    void solidRhombik();

};

#endif// _BRICK_H
