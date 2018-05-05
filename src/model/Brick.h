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

class Brick: public WhatUC<Brick>{
    static constexpr float side = 3.3f;
    static constexpr float depth = 1.6f;
    static constexpr float opacity = 0.7f; //when brick is hit
    static const int duration = 200; //hit effect

public:
    Uint32  hit_effect = 0;   // What happens when the brick is hit
    int     hit_counter;  // How many times has the brick been hit?
    int     type;         // Type of brick: normal, silver, gold: 0, 1, 2
    // Colors imported from 3DSMax
    Point3f rgb;

    int  hit();
    Brick(const Point3f& color,
          const Point3i& coords,
          int t);

   ~Brick();

    void display();
    Brick& animate(double secPerFrame);

private:
    static void solidRhombik(float side);
    static Point3f computeVertex(int tog,
                       int dir,
                       int dim,
                       int neg,
                       Point3f &vertex);
};

#endif// _BRICK_H
