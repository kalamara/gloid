#ifndef _BRICK_H
#define _BRICK_H

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
    Uint32  hit_effect;   // What happens when the brick is hit
    int     hit_counter;  // How many times has the brick been hit?
    int     type;         // Type of brick: normal, silver, gold: 0, 1, 2
    // Colors imported from 3DSMax
    point3f_t rgb;

    int  hit();
    Brick(const point3f_t color,
          const point3i_t coords,
          int t);

   ~Brick();

    void display();
    Brick& animate(double secPerFrame);

    static void solidRhombik(float side);
};

#endif// _BRICK_H
