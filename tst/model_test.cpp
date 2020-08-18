#include "GLoid.h"

#include "model/WhatUC.h"
#include "model/Particle.h"
#include "model/Crosshair.h"
#include "model/Ball.h"
#include "model/Shot.h"
#include "model/Vaus.h"
#include "model/Pill.h"
#include "model/Brick.h"
#include "model/Alien.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#define RED         {ONE, ZERO, ZERO}

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

//Game
template<> Engine<Game>::Engine(){
}

template<> Engine<Game>::~Engine(){
}

Game::Game(){
    vaus = std::make_unique<Vaus>(*this);
}

Game::~Game(){

}

Game & Game::loop(){
}

Step * Game::nextStep(){
}

std::optional<Brick> Game::getBrickAt(const Point3f& where, int axis){
    return {};
}

struct mousecntl MockMouse = mousecntl(400, 300, false);
struct screen MockScreen = screen(800, 600, 32, nullptr);
unsigned int MockTime = 100000;

template <> mousecntl_t Engine<Game>::getMouse() {
    mock().actualCall("Engine::getMouse");
    return &MockMouse;
}

template <> screenopt Engine<Game>::getScreen(){
    mock().actualCall("Engine::getScreen");
    return MockScreen;
}

template <> bool Engine<Game>::playSound(const std::string & sound) {
    mock().actualCall("Engine::playSound");
    return true;
    //  .withParameter("sound", WAV_LAUNCH);
}

template <> SDL_Surface * Engine<Game>::print2d(text2d & text) {
    mock().actualCall("Engine::print2d");
    return NULL;
}

template <> unsigned int Engine<Game>::toc() {
    mock().actualCall("Engine::toc");
    return MockTime;
}

TEST(ModelTestGroup, ParticleIsWhatUC){
    auto start_pos = Point3f(ONE, ONE, ONE);
    auto start_rot = Point3f(2*ONE, 2*ONE, 2*ONE);
    auto start_speed = Point3f(3*ONE, 3*ONE, 3*ONE);

    Point3f red = RED;

    mock().expectNCalls(10,"rand")
            .andReturnValue(15000499);
    //mock().expectNCalls(3,"Point3f::Point3f");
    auto p = Particle(start_pos, red, ONE);

    mock().checkExpectations();

    CHECK_EQUAL(500, p.life_total);
    DOUBLES_EQUAL(1.0f, p.life_fraction, FLOAT_PRECISION);

    //should explode randomly
    CHECK(p.active);
    CHECK(p.place.eq(Point3f(ONE, ONE, ONE)));

//    CHECK(p->speed.eq(Point3f(3.0f, 3.0f, 3.0f)));
//    CHECK(p->rotspeed.eq(Point3f(2.0f, 2.0f, 2.0f)));
//    CHECK(p->rotation.eq(Point3f(ONE, ONE, ONE)));

    DOUBLES_EQUAL(1.0f, p.side, FLOAT_PRECISION);
    CHECK(p.rgb.eq(Point3f(ONE, ZERO, ZERO)));

    //should have setters implemented
    p.setSize(1.0f, 2.0f, 3.0f)
     .setPlace(-1.0f, -2.0f, -3.0f);
    CHECK(p.size.eq(Point3f(ONE, 2.0f, 3.0f)));
    CHECK(p.place.eq(Point3f(-ONE, -2.0f, -3.0f)));

    //should have animate and mock display in place
    mock().expectOneCall("glPushMatrix");
    mock().expectOneCall("glTranslatef");
    mock().expectNCalls(3,"glRotatef");
    mock().expectOneCall("glColor4f");
    mock().expectOneCall("glBegin");
    mock().expectNCalls(3,"glVertex3f");
    mock().expectOneCall("glEnd");
    mock().expectOneCall("glPopMatrix");

    p.animate(0.01f).display();
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
    DOUBLES_EQUAL(0.98f, p.life_fraction, FLOAT_PRECISION);
    //when time expires particle is deactivated
    p.animate(0.49f);
    CHECK_FALSE(p.active)
    mock().checkExpectations();
}

TEST(ModelTestGroup, BallIsWhatUC){

    auto gm = Game();
    auto b =  Ball(gm);
    CHECK(b.active);
    CHECK_FALSE(b.launched);
    mock().expectNCalls(2,"glPushMatrix");
    mock().expectOneCall("glTranslatef");
    mock().expectOneCall("glColor3f");
    mock().expectNCalls(1,"gluSphere");
    CHECK(b.size.eq(Point3f(1.25f, 1.25f, 1.25f)));
    CHECK(b.speed.eq(Point3f()));
    CHECK(b.nextbounce.eq(Point3f()));
    CHECK(b.nextspeed.eq(Point3f()));

    mock().expectNCalls(2,"glPopMatrix");
    //should have animate and mock display in place
    //inactive, do nothing
    mock().expectOneCall("gluNewQuadric");
    mock().expectOneCall("gluDeleteQuadric");
    b.animate(0.01f).display();
}

TEST(ModelTestGroup, BallCollision){
    //ball vs. crosshairs
    auto gm = Game();
    auto b = Ball(gm);
    auto c = b.cross;
//degenerate: zero speed, does not collide
    b.speed = Point3f(ZERO, ZERO, ZERO);
    CHECK(!b.collides(c.place, c.size));

//degenerate: does not collide on itself
    b.speed = Point3f(ONE, ZERO, ZERO);
    CHECK(!b.collides(c.place, c.size));

    //collide when approaching
    CHECK(b.collides(Point3f(ONE, ZERO, ZERO), Point3f(ONE, ONE, ONE)))

    //don't collide when diverting
    CHECK(!b.collides(Point3f(-ONE, ZERO, ZERO), Point3f(ONE, ONE, ONE)))

    //don't collide when not close enough
    CHECK(!b.collides(Point3f(b.base_rad + FLOAT_PRECISION, ZERO, ZERO), Point3f(ZERO, ZERO, ZERO)))

}

TEST(ModelTestGroup, BallBouncing){
    auto gm = Game();
    auto b = Ball(gm);

    b.speed.deepcopy(Point3f(1.0f, 2.0f, 3.0f));
    //bounce on perpendicular planes
    b.bounce(AXIS_X);
    DOUBLES_EQUAL(-1.0f, b.speed.x, FLOAT_PRECISION);
    b.bounce(-AXIS_Y);
    DOUBLES_EQUAL(-2.0f, b.speed.y, FLOAT_PRECISION);
    b.bounce(AXIS_Z);
    DOUBLES_EQUAL(-3.0f, b.speed.z, FLOAT_PRECISION);

    //bounce on spherical model
    //degenerates
    b.speed.deepcopy(Point3f(0.0f, 0.0f, 0.0f));
    b.bounce(Point3f(ZERO, ZERO, ZERO), Point3f(ZERO, ZERO, ZERO));
    CHECK(b.speed.eq(Point3f()));

    //bounce on a sphere on axis X
    b.speed.x = ONE;
    b.bounce(Point3f(ONE, ZERO, ZERO), Point3f(ONE, ONE, ONE));
    DOUBLES_EQUAL(-ONE, b.speed.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, b.speed.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, b.speed.z, FLOAT_PRECISION);

    //speed on axis x, bounce on a sphere on axis Y => no bounce
    b.bounce(Point3f(ZERO, ONE, ZERO), Point3f(ONE, ONE, ONE));
    DOUBLES_EQUAL(-ONE, b.speed.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, b.speed.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, b.speed.z, FLOAT_PRECISION);

    //speed on Z, Y, bounce on a sphere on axis Z => get the resultant
    b.speed.deepcopy(Point3f(ZERO, ONE, ONE));
    b.bounce(Point3f(ZERO, ZERO, ONE), Point3f(ONE, ONE, ONE));
    DOUBLES_EQUAL(ZERO, b.speed.x, FLOAT_PRECISION);
    DOUBLES_EQUAL( ONE, b.speed.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(-ONE, b.speed.z, FLOAT_PRECISION);

    //same conditions but the sphere is scaled by 3/4 on Y
    b.speed.deepcopy(Point3f(ZERO, ROOT2, ROOT2));
    // (v2)^2 + (v2)^2 = 4
    // y^2 + z^2 = 4
    // z = y * 4/3
    // => ... y = 6/5, z = 8/5
    b.bounce(Point3f(ZERO, ZERO, ONE), Point3f(ONE, 4.0f, 3.0f));
    DOUBLES_EQUAL(ZERO, b.speed.x, FLOAT_PRECISION);
    DOUBLES_EQUAL( 1.2f, b.speed.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(-1.6f, b.speed.z, FLOAT_PRECISION);

    //random ricochet

    //degenerate: if speed is too small, reset initial speed
    mock().expectNCalls(3,"rand").andReturnValue(RAND_MAX / 2);
    b.bounce();
    mock().checkExpectations();
    CHECK(b.speed.eq(b.initspeed));

    //the norm remains the same
    auto n = b.speed.res3f();
    mock().expectNCalls(3,"rand").andReturnValue(0); //gives huge values but they are normalised
    b.bounce();
    mock().checkExpectations();

    DOUBLES_EQUAL(n,  b.speed.res3f(), FLOAT_PRECISION);

}

TEST(ModelTestGroup, BallAnimation){
   // mock().expectOneCall("gluNewQuadric");
    auto gm = Game();
    auto b = Ball(gm);
    //reinit inits
    Point3f init = {ONE, 2*ONE, 3*ONE};
    b.reinit(init);

    CHECK(b.active);
    CHECK_FALSE(b.launched);
    CHECK(b.speed.eq(Point3f()));
    CHECK(b.nextbounce.eq(Point3f()));
    CHECK(b.launchspeed.eq(Point3f(1.0f, 2.0f, 3.0f)));
    CHECK(b.nextspeed.eq(Point3f(1.0f, 2.0f, 3.0f)));
    //now it is displayable, but only animates following the vaus

    mock().expectNCalls(2,"glPushMatrix");
    mock().expectOneCall("glTranslatef");
    mock().expectOneCall("glColor3f");
    mock().expectOneCall("gluSphere");
    mock().expectNCalls(2,"glPopMatrix");
    mock().expectOneCall("gluNewQuadric");
    mock().expectOneCall("gluDeleteQuadric");

    b.animate(0.01f).display();
    mock().checkExpectations();
    //calculate projection (raycast) axis, speed and position
    //move following the vaus

    //launch launches

    mock().expectOneCall("Engine::playSound");
    // .withParameter("sound", WAV_LAUNCH);
    b.launch();

    CHECK(b.launched);
    CHECK(b.speed.eq(Point3f(1.0f, 2.0f, 3.0f)));
    mock().checkExpectations();


    //if it is trapped outside level, bring it back
    //if it is trapped inside a brick, bring it back
    //if  z < 0, lose
    //if on a brick, bounce
    //if on an alien, ricochet
    //if on vaus, bounce
    //if on wall, bounce


}

TEST(ModelTestGroup, VausIsWhatUC){
    auto gm = Game();
    auto v = Vaus(gm);

    CHECK(v.active);
    CHECK_FALSE(v.armed);
    CHECK_FALSE(v.large);
    CHECK_FALSE(v.warping);

    DOUBLES_EQUAL(5.0f, v.size.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(5.0f, v.size.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(1.25f, v.size.z, FLOAT_PRECISION);

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
    mock().expectOneCall("Engine::getScreen");
    mock().expectNCalls(1,"Engine::getMouse");
    mock().expectOneCall("gluNewQuadric");
    mock().expectOneCall("gluDeleteQuadric");
    v.animate(0.01f).display();

    DOUBLES_EQUAL(0.0f, v.place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(0.0f, v.place.y, FLOAT_PRECISION);
    mock().checkExpectations();
    //enlarge should enlarge and play enlarge
    gm.bonusMode = E;
    mock().expectOneCall("Engine::playSound");
    mock().expectOneCall("Engine::getScreen");
    mock().expectNCalls(1,"Engine::getMouse");
    //.withParameter("sound", WAV_ENLARGE);

    v.animate(0.01f);

    CHECK(v.large);
    mock().checkExpectations();

    gm.bonusMode = L;
    mock().expectOneCall("Engine::getScreen");
    mock().expectNCalls(1,"Engine::getMouse");

    v.animate(0.01f);

    CHECK(v.armed);
    mock().checkExpectations();
    gm.bonusMode = B;
    MockMouse.X = 800;
    MockMouse.Y = -600;

    //mock().expectOneCall("Engine::playSound");
    mock().expectOneCall("Engine::getScreen");
    mock().expectNCalls(1,"Engine::getMouse");

    v.animate(0.01f);

    DOUBLES_EQUAL(15.0f, v.place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(45.0f, v.place.y, FLOAT_PRECISION);
    CHECK_FALSE(v.warping);
    mock().checkExpectations();
}

TEST(ModelTestGroup, BrickIsWhatUC){
    Point3f red = RED;
    Point3i coords = {1,2,3};
    int t = BRIK_NORMAL;
    auto gm = Game();
    mock().expectOneCall("rand")
            .andReturnValue(1500);

    auto b = Brick(gm, red, coords, t);
    mock().checkExpectations();
    CHECK(b.active);
    CHECK(b.type == BRIK_NORMAL);
    CHECK(b.hit_counter == 1);
    DOUBLES_EQUAL(b.rgb.x, ONE, FLOAT_PRECISION);
    CHECK(b.hit_effect == 0);

    mock().expectOneCall("glEnable");
    mock().expectNCalls(2,"glDisable");
    mock().expectOneCall("glTranslatef");
    mock().expectOneCall("glScalef");
    mock().expectNCalls(2,"glRotatef");
    mock().expectOneCall("glColor3f");
    mock().expectNCalls(62,"glNormal3f");
    mock().expectNCalls(2,"glPushMatrix");
    mock().expectNCalls(2,"glPopMatrix");
    mock().expectNCalls(5, "glBegin");
    mock().expectNCalls(92,"glVertex3f");
    mock().expectNCalls(5,"glEnd");
    b.animate(0.01f).display();
    mock().checkExpectations();
}

TEST(ModelTestGroup, BrickHit){
    auto gm = Game();
    gm.brickCount = 3;
    mock().expectOneCall("rand")
            .andReturnValue(1500);
    auto r = Brick(gm, RED, {1,2,3}, BRIK_NORMAL);
    mock().checkExpectations();

    //normal brick is destroyed with one hit and increases score by SCOREBRICK
    r.hit();
    CHECK(!r.active);
    CHECK(r.pill.active);
    CHECK_EQUAL( SCOREBRICK, gm.score);
    CHECK_EQUAL(gm.brickCount, 2);

    //silver brick takes two hits to break and hit effect is active between the two
    mock().expectOneCall("rand")
            .andReturnValue(1500);
    auto s = Brick(gm, SILVER, {2,2,3}, BRIK_SILVER);
    mock().expectNCalls(1,"Engine::toc");
    s.hit();
    CHECK(s.active);
    CHECK_EQUAL(1, s.hit_counter);
    CHECK_EQUAL(MockTime, s.hit_effect);

    s.hit();
    CHECK(!s.active);
    CHECK(s.pill.active);
    CHECK_EQUAL( 2*SCOREBRICK, gm.score);
    CHECK_EQUAL(gm.brickCount, 1);
    mock().checkExpectations();

    //golden brick does not break, only hit effect plays
    mock().expectOneCall("rand")
            .andReturnValue(1500);
    auto g = Brick(gm, GOLD, {3,2,3}, BRIK_GOLDEN);
    mock().expectNCalls(1,"Engine::toc");
    g.hit();
    CHECK(g.active);
    CHECK_EQUAL(0, g.hit_counter);
    CHECK_EQUAL(MockTime, g.hit_effect);
    CHECK_EQUAL(gm.brickCount, 1);
    mock().checkExpectations();

}

TEST(ModelTestGroup, PillIsWhatUC){
    extern struct SDL_Surface MockSurf;

    auto gm = Game();

    Point3f start_pos = Point3f(ONE, ONE, -20.0f);

    mock().expectOneCall("rand")
            .andReturnValue(1500);

    memset(&MockSurf, 0, sizeof(struct SDL_Surface));

    auto p = make_unique<Pill>(start_pos, gm);
    //rand returned 1500 < RAND_MAX/20
    //and low scoring game => type should be L
    CHECK(!p->active)
    CHECK(p->type==L);
    mock().checkExpectations();
    p->active = true;

    mock().expectOneCall("Engine::getScreen");

    mock().expectNCalls(1, "SDL_CreateRGBSurface")
            .andReturnValue(&MockSurf);
    mock().expectOneCall("Engine::print2d");

    mock().expectNCalls(1, "SDL_MapRGBA");
//    mock().expectOneCall("SDL_SetColorKey");
    mock().expectOneCall("SDL_FillRect");
    mock().expectOneCall("SDL_UpperBlit");

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
    mock().expectNCalls(1,"gluNewQuadric");
    mock().expectNCalls(1,"gluDeleteQuadric");
    mock().expectNCalls(2, "SDL_FreeSurface");

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
    p->animate(0.01f);
    CHECK(!p->active);
}

TEST(ModelTestGroup, AlienIsWhatUC){
    auto gm = Game();
    auto a = Alien(gm);
    CHECK(a.place.eq(Point3f(ZERO, ZERO, SCENE_MIN - SCENE_MAX + ALIENHOME)));
    CHECK(a.size.eq(Point3f(4.0f, 4.0f, 4.0f)));
    CHECK(a.active);
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
    mock().expectNCalls(1,"gluNewQuadric");
    mock().expectNCalls(1,"gluDeleteQuadric");
    a.animate(0.01f).display();
    //no ball, speed is 0 on x, y, just rotate and approach player
    CHECK(a.place.eq(Point3f(ZERO, ZERO,
                              SCENE_MIN - SCENE_MAX + ALIENHOME + 0.05f)));
    DOUBLES_EQUAL(3.6f, a.roty,FLOAT_PRECISION);
}


//TODO: more alien display and animation tests
//TODO: explozans

TEST(ModelTestGroup, ShotIsWhatUC){

    auto gm = Game();
    auto s = Shot(gm, Point3f(ONE, ONE, ONE));

    mock().expectNCalls(8,"glPushMatrix");
    mock().expectNCalls(8,"glPopMatrix");
    mock().expectNCalls(4,"glTranslatef");
    mock().expectNCalls(4,"glScalef");
    mock().expectNCalls(4,"glColor3f");
    mock().expectNCalls(4,"gluSphere");
    mock().expectNCalls(1,"gluNewQuadric");
    mock().expectNCalls(1,"gluDeleteQuadric");
    //mock().expectOneCall("Game::getBrickAt");
    s.animate(0.01f).display();
    mock().checkExpectations();
    DOUBLES_EQUAL( 0.6, s.place.z, FLOAT_PRECISION);
}

TEST(ModelTestGroup, CrosshairIsWhatUC){

    auto gm = Game();
    auto c = Crosshair(gm);
    CHECK(c.size.eq(Point3f(1.0f, 1.0f, 0.0)));

    mock().expectNCalls(1,"Engine::toc");
    mock().expectNCalls(1,"glBindTexture");
    mock().expectNCalls(1,"glTexImage2D");
    mock().expectNCalls(2,"glTexParameteri");

    mock().expectNCalls(2,"glEnable");
    mock().expectNCalls(2,"glDisable");
    mock().expectNCalls(1,"glBegin");
    mock().expectNCalls(1,"glEnd");

    mock().expectNCalls(1,"glPushMatrix");
    mock().expectNCalls(1,"glPopMatrix");
    mock().expectNCalls(1,"glTranslatef");

    mock().expectNCalls(4,"glTexCoord2f");
    mock().expectNCalls(4,"glVertex3f");

    c.animate(0.01f).display();

    mock().checkExpectations();
}


//parsing levels
TEST(ModelTestGroup, parsing_test){

    auto game = Game();
    mock().checkExpectations();
    auto pos = Brick::getPos(std::string());
    CHECK(!pos);
    pos = Brick::getPos("");
    CHECK(!pos);
    pos = Brick::getPos("lol");
    CHECK(!pos);
    pos = Brick::getPos("TM_POS ");
    CHECK(!pos);
    pos = Brick::getPos("TM_POST ");
    CHECK(!pos);
    pos = Brick::getPos("TM_POST ROCK");
    CHECK(!pos);
    pos = Brick::getPos("TM_POS\tl");
    CHECK(!pos);
    pos = Brick::getPos("TM_POS\tl\t0\tl");
    CHECK(!pos);
    pos = Brick::getPos("TM_POS\t1.0\t20.0 ");
    CHECK(!pos);

    pos = Brick::getPos("TM_POS  0.0	60.0	90.0");
    CHECK(Point3i(0,6,9).eq(*pos));

    auto col = Brick::getColor("WIREFRAME_COLOR  0.3	 0.3	 0.3");
    CHECK(Point3f(0.3,0.3,0.3).eq(*col));

    std::stringstream bad("*BAD*input*");

    auto got = Brick::getBrick(bad, nullptr);
    CHECK(!got);
    std::string input = "\
*GEOMOBJECT {\
*TM_POS  0.0	 40.0	60.0\
*WIREFRAME_COLOR  0.3	 0.3	 0.3\
*GEOMOBJECT {\
*TM_POS 10.0	 0.0	60.0\
*WIREFRAME_COLOR  0.4	 0.4	 0.0\
*GEOMOBJECT {\
*TM_POS 10.0333	 1.234	60.00001\
*WIREFRAME_COLOR  0.1	 0.0	 0.0\
";

    std::stringstream iss(input);

    mock().expectOneCall("rand")
                    .andReturnValue(1500);

    got = Brick::getBrick(iss, &game);

    mock().checkExpectations();

    CHECK(got.has_value());
    CHECK(got.value().place.eq(Brick::fromBrick(Point3i(0,4,6))));
    CHECK(got.value().rgb.eq(Point3f(SILVER)));
    CHECK(got.value().type == BRIK_SILVER);

    mock().expectOneCall("rand")
                    .andReturnValue(1500);
    got = Brick::getBrick(iss, &game);
    mock().checkExpectations();

    CHECK(got.has_value());
    CHECK(got.value().place.eq(Brick::fromBrick(Point3i(1,0,6))));
    CHECK(got.value().rgb.eq(Point3f(GOLD)));
    CHECK(got.value().type == BRIK_GOLDEN);

    mock().expectOneCall("rand")
                    .andReturnValue(1500);
    got = Brick::getBrick(iss, &game);

    mock().checkExpectations();
    CHECK(got.has_value());
    CHECK(got.value().place.eq(Brick::fromBrick(Point3i(1,0,6))));
    CHECK(got.value().rgb.eq(Point3f(0.1,0,0)));
    CHECK(got.value().type == BRIK_NORMAL);
}

//coordinates conversions
TEST(ModelTestGroup, coords_test){
            Point3i i = Point3i(1,2,3);
            Point3f p = Brick::fromBrick(i);
            //f = 5x - 15  => x = 0.2f + 3
            DOUBLES_EQUAL(-10.0f, p.x, FLOAT_PRECISION);
            DOUBLES_EQUAL(-5.0f, p.y, FLOAT_PRECISION);
            //f = -2.5x -1.25 =>x = -0.4f - 0.5
            DOUBLES_EQUAL(-8.75f, p.z, FLOAT_PRECISION);
        //0,0,0 should be the center of the brick in (0,0,0)
        //    => (-15, -15, -1.25)
        // (half brick up left and half brick away from player)
            p = Brick::fromBrick(Point3i(0,0,0));
            DOUBLES_EQUAL(-15.0f, p.x, FLOAT_PRECISION);
            DOUBLES_EQUAL(-15.0f, p.y, FLOAT_PRECISION);
            DOUBLES_EQUAL(-1.25f, p.z, FLOAT_PRECISION);

            Point3i i2 = Brick::toBrick(p);
            CHECK_EQUAL(0, i2.X);
            CHECK_EQUAL(0, i2.Y);
            CHECK_EQUAL(0, i2.Z);

            Point3f p2;
            p2.deepcopy(p);
            p = Brick::fromBrick(Brick::toBrick(p2));

            CHECK(p.eq(p2));

            p = Brick::fromBrick(Point3i(6,6,12));
            DOUBLES_EQUAL(15.0f, p.x, FLOAT_PRECISION);
            DOUBLES_EQUAL(15.0f, p.y, FLOAT_PRECISION);
            DOUBLES_EQUAL(-31.25f, p.z, FLOAT_PRECISION);

            //CHECK(p.eq(Point3f(Point3i(p))));
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
