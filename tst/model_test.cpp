#include <list>

#include "GLoid.h"
#include "model/WhatUC.h"
#include "model/Particle.h"
#include "model/Ball.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "Functions.h"

using namespace std;

TEST_GROUP(ModelTestGroup){
    void teardown(){
            mock().clear();
    }
};

int rand() {
    return mock().actualCall("rand")
                 .returnIntValue();
}

point3f_t Functions::rand3f(float base) {
    return (point3f_t)mock().actualCall("Functions::rand3f")
                            .withParameter("base", 10.0f)
                            .returnPointerValue();
}

void glPushMatrix(){
    mock().actualCall("glPushMatrix");
}

void glPopMatrix(){
    mock().actualCall("glPopMatrix");
}

void glBegin(unsigned int mode){
    mock().actualCall("glBegin");
}

void glEnd(){
    mock().actualCall("glEnd");
}

void glTranslatef(float x, float y, float z){
    mock().actualCall("glTranslatef");
}

void glRotatef(float a, float x, float y, float z){
    mock().actualCall("glRotatef");
}

void glVertex3f(float x, float y, float z){
    mock().actualCall("glVertex3f");
}

void glColor4f( float x, float y, float z, float a){
    mock().actualCall("glColor4f");
}

TEST(ModelTestGroup, ParticleIsWhatUC){
    point3f_t start_pos = new point3f(ONE, ONE, ONE);
    point3f_t start_rot = new point3f(2*ONE, 2*ONE, 2*ONE);
    point3f_t start_speed = new point3f(3*ONE, 3*ONE, 3*ONE);

    point3f red = RED;

    mock().expectOneCall("rand")
          .andReturnValue(15000499);

    mock().expectOneCall("Functions::rand3f")
          .withParameter("base", 10.0f)
          .andReturnValue(start_speed);
    mock().expectOneCall("Functions::rand3f")
          .withParameter("base", 10.0f)
          .andReturnValue(start_rot);
    mock().expectOneCall("Functions::rand3f")
          .withParameter("base", 10.0f)
          .andReturnValue(start_pos);

    Particle *p = new Particle(start_pos, &red, ONE);

    mock().checkExpectations();

    CHECK_EQUAL(500, p->life_total);
    DOUBLES_EQUAL(1.0f, p->life_fraction, FLOAT_PRECISION);

    //should explode randomly
    DOUBLES_EQUAL(1.0f, p->place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, p->place.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, p->place.z, FLOAT_PRECISION);

    DOUBLES_EQUAL(3.0f, p->speed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(3.0f, p->speed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(3.0f, p->speed->z, FLOAT_PRECISION);

    DOUBLES_EQUAL(2.0f, p->rotspeed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f, p->rotspeed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f, p->rotspeed->z, FLOAT_PRECISION);

    DOUBLES_EQUAL(1.0f, p->rotation->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, p->rotation->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, p->rotation->z, FLOAT_PRECISION);

    DOUBLES_EQUAL(1.0f, p->side, FLOAT_PRECISION);

    DOUBLES_EQUAL(1.0f, p->rgb->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, p->rgb->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, p->rgb->z, FLOAT_PRECISION);

    //should have setters implemented
    *p = p->setSize(1.0f, 2.0f, 3.0f)
           .setPlace(-1.0f, -2.0f, -3.0f);

    DOUBLES_EQUAL(1.0f, p->size.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f, p->size.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(3.0f, p->size.z, FLOAT_PRECISION);
    DOUBLES_EQUAL(-1.0f, p->place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(-2.0f, p->place.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(-3.0f, p->place.z, FLOAT_PRECISION);

    //should have animate and mock display in place
    mock().expectOneCall("glPushMatrix");
    mock().expectOneCall("glTranslatef");
    mock().expectNCalls(3,"glRotatef");
    mock().expectOneCall("glColor4f");
    mock().expectOneCall("glBegin");
    mock().expectNCalls(3,"glVertex3f");
    mock().expectOneCall("glEnd");
    mock().expectOneCall("glPopMatrix");

    p->animate(0.01f).display();
    mock().checkExpectations();

    //x, y should increment, z should decrement by speed
    DOUBLES_EQUAL(-0.97f, p->place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(-1.97f, p->place.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(-3.03f, p->place.z, FLOAT_PRECISION);

    DOUBLES_EQUAL(1.02f, p->rotation->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.02f, p->rotation->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.98f, p->rotation->z, FLOAT_PRECISION);
    //remaining life should decrease by x seconds
    DOUBLES_EQUAL(0.98f, p->life_fraction, FLOAT_PRECISION);
    //when time expires particle is deactivated
    *p = p->animate(0.49f);
    CHECK_FALSE(p->active)
    delete p;
}

TEST(ModelTestGroup, BallIsWhatUC){
    Ball *b = new Ball();
    CHECK_FALSE(b->active);
    CHECK_FALSE(b->launched);

    DOUBLES_EQUAL(1.25f, b->size.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.25f, b->size.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.25f, b->size.z, FLOAT_PRECISION);

    DOUBLES_EQUAL(0.0f, b->speed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->speed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->speed->z, FLOAT_PRECISION);

    DOUBLES_EQUAL(0.0f, b->nextbounce->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->nextbounce->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->nextbounce->z, FLOAT_PRECISION);
    DOUBLES_EQUAL(10.0f, b->launchspeed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(10.0f, b->launchspeed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(20.0f, b->launchspeed->z, FLOAT_PRECISION);

    //should have animate and mock display in place
    b->animate(0.01f).display();


    delete b;
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
