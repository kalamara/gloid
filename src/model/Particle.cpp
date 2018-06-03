#include "GLoid.h"

#include "WhatUC.h"
#include "Particle.h"

Particle::Particle(const Point3f &where,
                   const Point3f &color,
                   float len){
    setPlace(where.x, where.y, where.z);
    life_total = rand() % life_max + 1;

    speed = Point3f(speed_max);
    rotspeed = Point3f(rot_max);
    rotation = Point3f(rot_max);
    rgb = Point3f(color);
    side = len;
}

Particle::~Particle(){
}

void Particle::display(){
    glPushMatrix();
        glTranslatef(place.x, place.y, place.z);
        glRotatef(rotation.x, ONE, ZERO, ZERO);
        glRotatef(rotation.y, ZERO, ONE, ZERO);
        glRotatef(rotation.z, ZERO, ZERO, ONE);
        glColor4f(rgb.x, rgb.y, rgb.z, life_fraction * life_fraction);
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(ZERO, side, ZERO);
            glVertex3f(ZERO,-side, ZERO);
            glVertex3f(side, ZERO, ZERO);
        glEnd();
    glPopMatrix();
}

Particle& Particle::animate(double secPerFrame){
    place.x += speed.x * secPerFrame;
    place.y += speed.y * secPerFrame;
    place.z -= speed.z * secPerFrame;

    rotation.x += rotspeed.x * secPerFrame;
    rotation.y += rotspeed.y * secPerFrame;
    rotation.z -= rotspeed.z * secPerFrame;

    life_fraction -= secPerFrame*1000/life_total;
    if(life_fraction < FLOAT_PRECISION){
          active = false;
    }
    return *this;
}
