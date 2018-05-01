//#include <list>

#include "GLoid.h"
#include "Point.h"
#include "Game.h"

#include "model/WhatUC.h"
#include "model/Particle.h"
#include "model/Ball.h"
#include "model/Vaus.h"
#include "model/Brick.h"
#include "model/Pill.h"
#include "model/Alien.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "GameMock.h"

#define RED         {1.0f, 0.0f, 0.0f}

using namespace std;

TEST_GROUP(ModelTestGroup){
    void teardown(){
        mock().clear();
    }
};
//C++ libraries
int rand() {
    return mock().actualCall("rand")
            .returnIntValue();
}
//Functions
/*
Point3f::Point3f(float base){
    mock().actualCall("Point3f::Point3f");
}
*/
Point3f::Point3f(const Point3i& other){
    mock().actualCall("Point3f::Point3f");
}
/*
float Point3f::res3f(){
    return mock().actualCall("Point3f::res3f")
            .returnDoubleValue();
}
*/
Point3f Point3f::chase(const Point3f& other, float U){
    mock().actualCall("Point3f::chase");
}

//Game
void Game::playSound(int sound) {
    mock().actualCall("Game::playSound");
    //  .withParameter("sound", WAV_LAUNCH);
}

int Game::now() {
    mock().actualCall("Game::now");
}

bool Game::isHiScoring() {
    mock().actualCall("Game::isHiScoring")
            .returnBoolValue();
}

unsigned int Game::levelType(){
    mock().actualCall("Game::levelType")
            .returnIntValue();
    return 0;
}

Ball * Game::getActiveBall(){
    mock().actualCall("Game::getActiveBall");
    return NULL;
}

Brick * Game::getBrickAt(Point3f &where){
    mock().actualCall("Game::getBrickAt");
    return NULL;
}

void gluQuadricOrientation(GLUquadric* quad, GLenum orientation){
    mock().actualCall("gluQuadricOrientation");
}

void gluDisk(GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops){
    mock().actualCall("gluDisk");
}

void Game::printText(bool option,
                     text2d* text,
                     SDL_Color fg,
                     SDL_Color bg,
                     int x,
                     int y,
                     const char* buf, ...){
    mock().actualCall("Game::printText");
}

void Game::setBonusMode(int type){
    mock().actualCall("Game::setBonusMode");
}

void Game::incLives(){
    mock().actualCall("Game::setBonusMode");
}

void Game::divideBalls(){
    mock().actualCall("Game::divideBalls");
}

//SDL
struct SDL_Surface Surf;

SDL_Surface * SDL_CreateRGBSurface
(Uint32 flags, int width, int height, int depth,
 Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask){
    return (SDL_Surface *)mock().actualCall("SDL_CreateRGBSurface")
            .returnPointerValueOrDefault(&Surf);
}

int SDL_SetColorKey
(SDL_Surface *surface, Uint32 flag, Uint32 key){
    mock().actualCall("SDL_SetColorKey");
}

int SDL_FillRect
(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color){
    mock().actualCall("SDL_FillRect");
}

int SDL_UpperBlit
(SDL_Surface *src, SDL_Rect *srcrect,
 SDL_Surface *dst, SDL_Rect *dstrect){
    mock().actualCall("SDL_UpperBlit");
}

void SDL_FreeSurface(SDL_Surface *surface){
    mock().actualCall("SDL_FreeSurface");
}
Uint32 SDL_MapRGBA
(const SDL_PixelFormat * const format,
 const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a){
    mock().actualCall("SDL_MapRGBA");
}

//OpenGL
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

void glTexEnvf( GLenum target, GLenum pname, GLfloat param ){
    mock().actualCall("glTexEnvf");
}

void glTexParameteri( GLenum target, GLenum pname, GLint param ){
    mock().actualCall("glTexParameteri");
}

void glTexImage2D( GLenum target, GLint level,
                   GLint internalFormat,
                   GLsizei width, GLsizei height,
                   GLint border, GLenum format, GLenum type,
                   const GLvoid *pixels ){
    mock().actualCall("glTexImage2D");
}

void glMatrixMode( GLenum mode ){
    mock().actualCall("glMatrixMode");
}

//OpenGL extensions
GLUquadric * gluNewQuadric(){
    mock().actualCall("gluNewQuadric");
}

void gluDeleteQuadric(GLUquadric * base){
    mock().actualCall("gluDeleteQuadric");
}

void gluQuadricDrawStyle (GLUquadric* quad, GLenum draw){
    mock().actualCall("gluQuadricDrawStyle");
}

void gluQuadricTexture (GLUquadric* quad, GLboolean texture){
    mock().actualCall("gluQuadricTexture");
}

void gluSphere( GLUquadric * b, double r, int sl, int st){
    mock().actualCall("gluSphere");
}

void gluCylinder (GLUquadric* quad, double base, double top, double height, int slices, int stacks){
    mock().actualCall("gluCylinder");
}


TEST(ModelTestGroup, ParticleIsWhatUC){
    Point3f start_pos = Point3f(ONE, ONE, ONE);
    Point3f start_rot = Point3f(2*ONE, 2*ONE, 2*ONE);
    Point3f start_speed = Point3f(3*ONE, 3*ONE, 3*ONE);

    Point3f red = RED;

    mock().expectNCalls(10,"rand")
            .andReturnValue(15000499);
    //mock().expectNCalls(3,"Point3f::Point3f");
    Particle *p = new Particle(start_pos, red, ONE);

    mock().checkExpectations();

    CHECK_EQUAL(500, p->life_total);
    DOUBLES_EQUAL(1.0f, p->life_fraction, FLOAT_PRECISION);

    //should explode randomly
    CHECK(p->active);
    CHECK(p->place.eq(Point3f(ONE, ONE, ONE)));

//    CHECK(p->speed.eq(Point3f(3.0f, 3.0f, 3.0f)));
//    CHECK(p->rotspeed.eq(Point3f(2.0f, 2.0f, 2.0f)));
//    CHECK(p->rotation.eq(Point3f(ONE, ONE, ONE)));

    DOUBLES_EQUAL(1.0f, p->side, FLOAT_PRECISION);
    CHECK(p->rgb.eq(Point3f(ONE, ZERO, ZERO)));

    //should have setters implemented
    *p = p->setSize(1.0f, 2.0f, 3.0f)
            .setPlace(-1.0f, -2.0f, -3.0f);
    CHECK(p->size.eq(Point3f(ONE, 2.0f, 3.0f)));
    CHECK(p->place.eq(Point3f(-ONE, -2.0f, -3.0f)));

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
    /*DOUBLES_EQUAL(-0.97f, p->place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(-1.97f, p->place.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(-3.03f, p->place.z, FLOAT_PRECISION);

    DOUBLES_EQUAL(1.02f, p->rotation.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.02f, p->rotation.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.98f, p->rotation.z, FLOAT_PRECISION);
    //remaining life should decrease by x seconds
    */
    DOUBLES_EQUAL(0.98f, p->life_fraction, FLOAT_PRECISION);
    //when time expires particle is deactivated
    *p = p->animate(0.49f);
    CHECK_FALSE(p->active)
    delete p;
    mock().checkExpectations();
}

TEST(ModelTestGroup, BallIsWhatUC){
    mock().expectOneCall("gluNewQuadric");
    Ball *b = new Ball();
    CHECK_FALSE(b->active);
    CHECK_FALSE(b->launched);

    CHECK(b->size.eq(Point3f(1.25f, 1.25f, 1.25f)));
    CHECK(b->speed.eq(Point3f()));
    CHECK(b->nextbounce.eq(Point3f()));
    CHECK(b->nextspeed.eq(Point3f()));
    CHECK(b->launchspeed.eq(Point3f(10.0f, 10.0f, 20.0f)));

    //should have animate and mock display in place
    //inactive, do nothing
    b->animate(0.01f).display();
    mock().expectOneCall("gluDeleteQuadric");
    delete b;
    mock().checkExpectations();
}

TEST(ModelTestGroup, BallAnimation){
    mock().expectOneCall("gluNewQuadric");
    Ball *b = new Ball();
    //reinit inits
    Point3f init = {ONE, 2*ONE, 3*ONE};
    *b = b->reinit(init);

    CHECK(b->active);
    CHECK_FALSE(b->launched);
    CHECK(b->speed.eq(Point3f()));
    CHECK(b->nextbounce.eq(Point3f()));
    CHECK(b->launchspeed.eq(Point3f(1.0f, 2.0f, 3.0f)));
    CHECK(b->nextspeed.eq(Point3f(1.0f, 2.0f, 3.0f)));
    //now it is displayable, but only animates following the vaus

    mock().expectNCalls(2,"glPushMatrix");
    mock().expectOneCall("glTranslatef");
    mock().expectOneCall("glColor3f");
    mock().expectOneCall("gluSphere");
    mock().expectNCalls(2,"glPopMatrix");

    b->animate(0.01f).display();
    mock().checkExpectations();
    //calculate projection (raycast) axis, speed and position
    //move following the vaus

    //launch launches

    mock().expectOneCall("Game::playSound");
    // .withParameter("sound", WAV_LAUNCH);
    *b = b->launch();

    CHECK(b->launched);
    CHECK(b->speed.eq(Point3f(1.0f, 2.0f, 3.0f)));
    mock().checkExpectations();

    //if it is trapped outside level, bring it back
    //if it is trapped inside a brick, bring it back
    //if  z < 0, lose
    //if on a brick, bounce
    //if on an alien, ricochet
    //if on vaus, bounce
    //if on wall, bounce
    mock().expectOneCall("gluDeleteQuadric");
    delete b;
    mock().checkExpectations();
}

TEST(ModelTestGroup, VausIsWhatUC){
    GameMock * gm = new GameMock();//GameMock::instance();
    mousecntl_t mou = new mousecntl(400,300, FALSE);
    screen_t scr = new screen(800, 600, 32);
    mock().expectOneCall("gluNewQuadric");
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
    mock().expectOneCall("gluDeleteQuadric");
    delete v;
    delete gm;
    mock().checkExpectations();
}

TEST(ModelTestGroup, BrickIsWhatUC){
    Point3f red = RED;
    Point3i coords = {1,2,3};
    int t = BRIK_NORMAL;

    mock().expectOneCall("Point3f::Point3f");
    Brick * b = new Brick(red, coords, t);
    mock().checkExpectations();
    CHECK(b->active);
    CHECK(b->type == BRIK_NORMAL);
    CHECK(b->hit_counter == 1);
    DOUBLES_EQUAL(b->rgb.x, ONE, FLOAT_PRECISION);
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

TEST(ModelTestGroup, PillIsWhatUC){
    GameMock * gm = new GameMock();
    mock().expectOneCall("gluNewQuadric");
    Vaus *v = new Vaus(gm);
    Point3f start_pos = Point3f(ONE, ONE, -20.0f);
    gm->vaus = v;

    mock().expectOneCall("Game::isHiScoring")
            .andReturnValue(false);
    mock().expectOneCall("rand")
            .andReturnValue(1500);

    memset(&Surf, 0, sizeof(struct SDL_Surface));
    mock().expectNCalls(2, "SDL_CreateRGBSurface")
            .andReturnValue(&Surf);

    mock().expectNCalls(2, "SDL_MapRGBA");
    mock().expectOneCall("SDL_SetColorKey");
    mock().expectOneCall("SDL_FillRect");
    mock().expectOneCall("Game::printText");
    mock().expectOneCall("SDL_UpperBlit");
    mock().expectOneCall("gluNewQuadric");
    Pill *p = new Pill(start_pos, gm);
    //rand returned 1500 < RAND_MAX/20
    //and low scoring game => type should be L
    CHECK(p->active)
    CHECK(p->type==L);
    mock().checkExpectations();

    mock().expectOneCall("glEnable");
    mock().expectOneCall("glTexEnvf");
    mock().expectOneCall("glDisable");
    mock().expectNCalls(2,"glMatrixMode");
    mock().expectOneCall("glTexImage2D");
    mock().expectOneCall("glTexParameteri");
    mock().expectNCalls(6,"glPushMatrix");
    mock().expectNCalls(4,"glRotatef");
    mock().expectNCalls(4,"glTranslatef");
    mock().expectNCalls(2,"glColor3f");
    mock().expectOneCall("gluCylinder");
    mock().expectNCalls(2,"gluSphere");
    mock().expectNCalls(6,"glPopMatrix");
    mock().expectOneCall("gluQuadricTexture");

    p->animate(0.01f).display();
    mock().checkExpectations();

    //should approach player at speed 10.0 * 0.01
    //started at 20.0f
    DOUBLES_EQUAL(p->place.z, -19.9f, FLOAT_PRECISION);
    //should rotate at speed 360.0 * 0.01
    DOUBLES_EQUAL(p->rotx, 3.6f, FLOAT_PRECISION);

    p->setPlace(ONE, ONE, ZERO);
    //pill has reached vaus
    mock().expectOneCall("Game::setBonusMode");
    *p = p->animate(0.01f);
    CHECK(!p->active);


    mock().expectNCalls(2, "SDL_FreeSurface");
    mock().expectOneCall("gluDeleteQuadric");
    delete p;
    mock().checkExpectations();

    mock().expectOneCall("gluDeleteQuadric");
    delete v;
    delete gm;
    mock().checkExpectations();
}

TEST(ModelTestGroup, AlienIsWhatUC){
    GameMock * gm = new GameMock();
    mock().expectNCalls(2,"gluNewQuadric");
    Vaus *v = new Vaus(gm);
    gm->vaus = v;
    Alien *a = new Alien(gm);
    CHECK(a->place.eq(Point3f(ZERO, ZERO, SCENE_MIN - SCENE_MAX + ALIENHOME)));
    CHECK(a->size.eq(Point3f(4.0f, 4.0f, 4.0f)));
    CHECK(a->active);
    mock().expectOneCall("Game::getActiveBall");
    mock().expectOneCall("Game::getBrickAt");
    mock().expectOneCall("Game::levelType");
    //leveltype is 0, alien type 1 (blue double cone)
    mock().expectNCalls(4,"glPushMatrix");
    mock().expectNCalls(4,"glPopMatrix");
    mock().expectOneCall("glColor3f");
    mock().expectNCalls(4,"glTranslatef");
    mock().expectNCalls(3,"glRotatef");
    mock().expectNCalls(2,"gluQuadricDrawStyle");
    mock().expectNCalls(2,"gluCylinder");
    mock().expectNCalls(4,"gluQuadricOrientation");
    mock().expectNCalls(3,"gluDisk");
    a->animate(0.01f).display();
    //no ball, speed is 0 on x, y, just rotate and approach player
    CHECK(a->place.eq(Point3f(ZERO, ZERO,
                              SCENE_MIN - SCENE_MAX + ALIENHOME + 0.05f)));
    DOUBLES_EQUAL(3.6f, a->roty,FLOAT_PRECISION);

    mock().expectNCalls(2,"gluDeleteQuadric");
    delete v;
    delete gm;
    delete a;
    mock().checkExpectations();
}

//TODO: more alien display and animation tests
//TODO: explozans
int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
