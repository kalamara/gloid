#include <list>

#include "GLoid.h"
#include "model/WhatUC.h"
#include "model/Particle.h"

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

TEST(ModelTestGroup, ParticleIsWhatUC){
    point3f_t start_pos = new point3f(EYE, EYE, EYE);
    point3f_t start_rot = new point3f(2*EYE, 2*EYE, 2*EYE);
    point3f_t start_speed = new point3f(3*EYE, 3*EYE, 3*EYE);

    point3f red = RED;

    mock().expectOneCall("rand")
          .andReturnValue(1500001);

    mock().expectOneCall("Functions::rand3f")
          .withParameter("base", 10.0f)
          .andReturnValue(start_pos);
    mock().expectOneCall("Functions::rand3f")
          .withParameter("base", 10.0f)
          .andReturnValue(start_rot);
    mock().expectOneCall("Functions::rand3f")
          .withParameter("base", 10.0f)
          .andReturnValue(start_speed);

    Particle *p = new Particle(start_pos, &red, EYE);

    mock().checkExpectations();

    CHECK_EQUAL(1, p->life_total);
    DOUBLES_EQUAL(1.0f, p->life_fraction, FLOAT_PRECISION);

    //should explode randomly
    DOUBLES_EQUAL(1.0f, p->place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, p->place.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, p->place.z, FLOAT_PRECISION);

    DOUBLES_EQUAL(1.0f, p->speed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, p->speed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, p->speed->z, FLOAT_PRECISION);

    DOUBLES_EQUAL(2.0f, p->rotspeed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f, p->rotspeed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f, p->rotspeed->z, FLOAT_PRECISION);

    DOUBLES_EQUAL(3.0f, p->rotation->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(3.0f, p->rotation->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(3.0f, p->rotation->z, FLOAT_PRECISION);

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
    p->animate(40.0f).display();


    delete p;
}


int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
