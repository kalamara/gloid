#ifndef _PARTICLE_H_
#define _PARTICLE_H_

class Particle: public WhatUC<Particle>{
    static const int life_max = 1500;
    static constexpr float speed_max = 10.0f;
    static constexpr float rot_max = 10.0f * FULL_CIRCLE;
public:
    point3f_t speed;
    point3f_t rotation;
    point3f_t rotspeed;
    point3f_t rgb;

    float life_fraction;  // From 0 to 1
    int life_total;       // In milliseconds

    Particle(const point3f_t where,
            const point3f_t pal,
            int palette_size,
            float side);
    ~Particle();
    void display();
    Particle& animate(double secPerFrame);
    //Particle& explode(point3f, point3f*, int, float);

};

#endif //_PARTICLE_H_
