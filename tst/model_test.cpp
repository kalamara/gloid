//#include <list>

#include "GLoid.h"
#include "Functions.h"
#include "Game.h"
#include "model/WhatUC.h"
#include "model/Particle.h"
#include "model/Ball.h"
#include "model/Vaus.h"
#include "model/Brick.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "GameMock.h"


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

point3f_t Functions::fromcoords(point3i_t coords){
    //point3i p = {1,2,3};
    return (point3f_t)mock().actualCall("Functions::fromcoords")
            .withParameter("coords", (void *)coords)
            .returnPointerValue();
}

void Game::playSound(int sound) {
    mock().actualCall("Game::playSound");
          //  .withParameter("sound", WAV_LAUNCH);
}

int Game::now() {
    mock().actualCall("Game::now");
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

void glEnable( GLenum cap ){
    mock().actualCall("glEnable");
}

void glDisable( GLenum cap ){
    mock().actualCall("glDisable");
}

void glScalef( GLfloat x, float y, float z ){
    mock().actualCall("glScalef");
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

void glColor3f( float x, float y, float z){
    mock().actualCall("glColor3f");
}

void glNormal3f( float x, float y, float z){
    mock().actualCall("glNormal3f");
}

GLUquadric * gluNewQuadric(){
    mock().actualCall("gluNewQuadric");
}

void gluDeleteQuadric(GLUquadric * base){
    mock().actualCall("gluDeleteQuadric");
}

void gluQuadricDrawStyle (GLUquadric* quad, GLenum draw){
    mock().actualCall("gluQuadricDrawStyle");
}

void gluSphere( GLUquadric * b, double r, int sl, int st){
    mock().actualCall("gluSphere");
}

void gluCylinder (GLUquadric* quad, double base, double top, double height, int slices, int stacks){
    mock().actualCall("gluCylinder");
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
    CHECK(p->active);
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
    DOUBLES_EQUAL(0.0f, b->nextspeed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->nextspeed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->nextspeed->z, FLOAT_PRECISION);
    DOUBLES_EQUAL(10.0f, b->launchspeed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(10.0f, b->launchspeed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(20.0f, b->launchspeed->z, FLOAT_PRECISION);


    //should have animate and mock display in place
    //inactive, do nothing
    b->animate(0.01f).display();

    delete b;
}

TEST(ModelTestGroup, BallAnimation){
    Ball *b = new Ball();
    //reinit inits
    point3f init = {ONE, 2*ONE, 3*ONE};
    *b = b->reinit(&init);

    CHECK(b->active);
    CHECK_FALSE(b->launched);
    DOUBLES_EQUAL(0.0f, b->speed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->speed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->speed->z, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->nextbounce->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->nextbounce->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, b->nextbounce->z, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, b->launchspeed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f, b->launchspeed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(3.0f, b->launchspeed->z, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.0f, b->nextspeed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f, b->nextspeed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(3.0f, b->nextspeed->z, FLOAT_PRECISION);

    //now it is displayable, but only animates following the vaus

    mock().expectOneCall("gluNewQuadric");
    mock().expectNCalls(2,"glPushMatrix");
    mock().expectOneCall("glTranslatef");
    mock().expectOneCall("glColor3f");
    mock().expectOneCall("gluSphere");
    mock().expectNCalls(2,"glPopMatrix");
    mock().expectOneCall("gluDeleteQuadric");

    b->animate(0.01f).display();
    mock().checkExpectations();
    //calculate projection (raycast) axis, speed and position
    //move following the vaus

    //launch launches

    mock().expectOneCall("Game::playSound");
           // .withParameter("sound", WAV_LAUNCH);
    *b = b->launch();

    CHECK(b->launched);
    DOUBLES_EQUAL(1.0f, b->speed->x, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f, b->speed->y, FLOAT_PRECISION);
    DOUBLES_EQUAL(3.0f, b->speed->z, FLOAT_PRECISION);
    mock().checkExpectations();

    //if it is trapped outside level, bring it back
    //if it is trapped inside a brick, bring it back
    //if  z < 0, lose
    //if on a brick, bounce
    //if on an alien, ricochet
    //if on vaus, bounce
    //if on wall, bounce

    delete b;
}

TEST(ModelTestGroup, VausIsWhatUC){
    GameMock * gm = new GameMock();//GameMock::instance();
    mousecntl_t mou = new mousecntl(400,300, FALSE);
    screen_t scr = new screen(800, 600, 32);
    Vaus *v = new Vaus(gm);
    //
    gm->mouse = mou;
    gm->scr = scr;

    CHECK(v->active);
    CHECK_FALSE(v->armed);
    CHECK_FALSE(v->large);
    CHECK_FALSE(v->warping);

    DOUBLES_EQUAL(5.0f, v->size.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(5.0f, v->size.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.25f, v->size.z, FLOAT_PRECISION);

    mock().expectOneCall("gluNewQuadric");
    mock().expectOneCall("gluQuadricDrawStyle");
    mock().expectNCalls(10,"glPushMatrix");
    mock().expectNCalls(10,"glTranslatef");
    mock().expectNCalls(5,"glRotatef");
    mock().expectOneCall("glScalef");
    mock().expectOneCall("glColor4f");
    mock().expectNCalls(5,"gluSphere");
    mock().expectOneCall("glEnable");
    mock().expectOneCall("glDisable");
    mock().expectNCalls(10,"glPopMatrix");
    mock().expectNCalls(8,"glColor3f");
    mock().expectNCalls(4,"gluCylinder");
    mock().expectOneCall("gluDeleteQuadric");
    v->animate(0.01f).display();

    DOUBLES_EQUAL(0.0f, v->place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(30.0f, v->place.y, FLOAT_PRECISION);
    mock().checkExpectations();
    //enlarge should enlarge and play enlarge
    gm->bonusMode = E;
    mock().expectOneCall("Game::playSound");
           //.withParameter("sound", WAV_ENLARGE);

    *v = v->animate(0.01f);
    CHECK(v->large);
    mock().checkExpectations();

    gm->bonusMode = L;
    *v = v->animate(0.01f);
    CHECK(v->armed);
    mock().checkExpectations();

    gm->mouse->X = 800;
    gm->mouse->Y = -600;

    gm->bonusMode = B;
    mock().expectOneCall("Game::playSound");

    *v = v->animate(0.01f);

    DOUBLES_EQUAL(15.0f, v->place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(-15.0f, v->place.y, FLOAT_PRECISION);
    CHECK(v->warping);
    mock().checkExpectations();

    delete mou;
    delete scr;
    delete v;
    delete gm;
}

TEST(ModelTestGroup, BrickIsWhatUC){
    point3f red = RED;
    point3i coords = {1,2,3};
    point3f_t where = new point3f();
    int t = BRIK_NORMAL;
    mock().expectOneCall("Functions::fromcoords")
            .withParameter("coords", &coords)
            .andReturnValue((void *)where);

    Brick * b = new Brick(&red, &coords, t);

    mock().checkExpectations();
    CHECK(b->active);
    CHECK(b->type == BRIK_NORMAL);
    CHECK(b->hit_counter == 1);
    DOUBLES_EQUAL(b->rgb->x, 1.0f, FLOAT_PRECISION);
    CHECK(b->hit_effect == 0);

    mock().expectOneCall("glEnable");
    mock().expectNCalls(2,"glDisable");
    mock().expectOneCall("glScalef");
    mock().expectNCalls(2,"glRotatef");
    mock().expectOneCall("glColor3f");
    mock().expectNCalls(62,"glNormal3f");
    mock().expectNCalls(1,"glPushMatrix");
    mock().expectNCalls(1,"glPopMatrix");
    mock().expectNCalls(5, "glBegin");
    mock().expectNCalls(92,"glVertex3f");
    mock().expectNCalls(5,"glEnd");
    b->animate(0.01f).display();
    mock().checkExpectations();

    delete b;
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
