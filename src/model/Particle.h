#ifndef _PARTICLE_H_
#define _PARTICLE_H_

class Particle: public WhatUC<Particle>{
    static const int life_max = 1500; //milliseconds
    static constexpr float speed_max = 10.0f; //per frame
    static constexpr float rot_max = 0.1f * FULL_CIRCLE; //per frame
public:
    float side;
    point3f_t speed;
    point3f_t rotation;
    point3f_t rotspeed;
    point3f_t rgb;

    float life_fraction;  // From 0 to 1
    int life_total;       // In milliseconds

    Particle(const point3f_t where,
            const point3f_t color,
            float len);
    ~Particle();
    void display();
    Particle& animate(double secPerFrame);
    //Particle& explode(point3f, point3f*, int, float);

};

#endif //_PARTICLE_H_
