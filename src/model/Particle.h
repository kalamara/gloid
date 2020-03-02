#ifndef _PARTICLE_H_
#define _PARTICLE_H_
class Point3f;

class Particle: public WhatUC<Particle>{
    static const int life_max = 1500; //milliseconds
    static constexpr float speed_max = 10.0f; //per frame
    static constexpr float rot_max = 0.2f * HALF_CIRCLE; //per frame
public:
    float side;
 //   Point3f speed;
    Point3f rotation;
    Point3f rotspeed;
    Point3f rgb;

    float life_fraction = ONE;  // From 0 to 1
    int life_total;       // In milliseconds

    Particle(const Point3f & where,
            const Point3f & color,
            float len);
    ~Particle();
    void display();
    Particle& animate(double secPerFrame);
    //Particle& explode(point3f, point3f*, int, float);

};

#endif //_PARTICLE_H_
