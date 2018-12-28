#include "GLoid.h"
#include "game/Play.h"

#include "model/WhatUC.h"
#include "model/Particle.h"
#include "model/Ball.h"
#include "model/Vaus.h"
#include "model/Brick.h"
#include "model/Pill.h"
#include "model/Alien.h"
#include "model/Shot.h"

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

}

Game::~Game(){

}

Game & Game::loop(){
}

Step & Game::nextStep(){
}


Play::Play(){

}

Play::~Play(){

}

Play & Play::draw(){

}

Play & Play::update(){

}

Play & Play::next(){

}

struct mousecntl MockMouse = mousecntl(400, 300, false);
struct screen MockScreen = screen(800, 600, 32, nullptr);

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
    //  .withParameter("sound", WAV_LAUNCH);
}

template <> int Engine<Game>::toc() {
    mock().actualCall("Engine::now");
}

Ball * Game::getActiveBall(){
    mock().actualCall("Game::getActiveBall");
    return nullptr;
}

Brick * Game::getBrickAt(Point3f &where){
    mock().actualCall("Game::getBrickAt");
    return nullptr;
}

void Game::divideBalls(){
    mock().actualCall("Game::divideBalls");
}

void Game::killVaus(){
    mock().actualCall("Game::killVaus");
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
    mock().expectOneCall("gluNewQuadric");
    auto gm = Game();
    auto b =  Ball(gm);
    CHECK_FALSE(b.active);
    CHECK_FALSE(b.launched);

    CHECK(b.size.eq(Point3f(1.25f, 1.25f, 1.25f)));
    CHECK(b.speed.eq(Point3f()));
    CHECK(b.nextbounce.eq(Point3f()));
    CHECK(b.nextspeed.eq(Point3f()));
    CHECK(b.launchspeed.eq(Point3f(10.0f, 10.0f, 20.0f)));

    //should have animate and mock display in place
    //inactive, do nothing
    b.animate(0.01f).display();
    mock().expectOneCall("gluDeleteQuadric");
}

TEST(ModelTestGroup, BallAnimation){
    mock().expectOneCall("gluNewQuadric");
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
    mock().expectOneCall("gluDeleteQuadric");
}

TEST(ModelTestGroup, VausIsWhatUC){
    auto gm = Game();

    mock().expectOneCall("gluNewQuadric");

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

    v.animate(0.01f).display();

    DOUBLES_EQUAL(0.0f, v.place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(30.0f, v.place.y, FLOAT_PRECISION);
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

    mock().expectOneCall("Engine::playSound");
    mock().expectOneCall("Engine::getScreen");
    mock().expectNCalls(1,"Engine::getMouse");

    v.animate(0.01f);

    DOUBLES_EQUAL(15.0f, v.place.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(-15.0f, v.place.y, FLOAT_PRECISION);
    CHECK(v.warping);
    mock().checkExpectations();

    mock().expectOneCall("gluDeleteQuadric");
}

TEST(ModelTestGroup, BrickIsWhatUC){
    Point3f red = RED;
    Point3i coords = {1,2,3};
    int t = BRIK_NORMAL;
    auto gm = Game();

    auto b = Brick(gm, red, coords, t);
    mock().checkExpectations();
    CHECK(b.active);
    CHECK(b.type == BRIK_NORMAL);
    CHECK(b.hit_counter == 1);
    DOUBLES_EQUAL(b.rgb.x, ONE, FLOAT_PRECISION);
    CHECK(b.hit_effect == 0);

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
    b.animate(0.01f).display();
    mock().checkExpectations();
}

TEST(ModelTestGroup, PillIsWhatUC){
    extern struct SDL_Surface Surf;

    auto gm = Game();

    mock().expectNCalls(1,"gluNewQuadric");
    auto v = Vaus(gm);
    gm.setVaus(&v);
    Point3f start_pos = Point3f(ONE, ONE, -20.0f);

    mock().expectOneCall("rand")
            .andReturnValue(1500);

    memset(&Surf, 0, sizeof(struct SDL_Surface));
    mock().expectNCalls(2, "SDL_CreateRGBSurface")
            .andReturnValue(&Surf);

    mock().expectNCalls(2, "SDL_MapRGBA");
    mock().expectOneCall("SDL_SetColorKey");
    mock().expectOneCall("SDL_FillRect");
    mock().expectOneCall("SDL_UpperBlit");
    mock().expectOneCall("gluNewQuadric");
    mock().expectOneCall("Engine::getScreen");
    auto p = make_unique<Pill>(start_pos, gm);
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
    p->animate(0.01f);
    CHECK(!p->active);

    mock().expectNCalls(1,"gluDeleteQuadric");
    mock().expectNCalls(2, "SDL_FreeSurface");
    mock().expectNCalls(1,"gluDeleteQuadric");
}

TEST(ModelTestGroup, AlienIsWhatUC){
    auto gm = Game();
    mock().expectNCalls(2,"gluNewQuadric");
    auto v = Vaus(gm);
    gm.setVaus(&v);
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

    a.animate(0.01f).display();
    //no ball, speed is 0 on x, y, just rotate and approach player
    CHECK(a.place.eq(Point3f(ZERO, ZERO,
                              SCENE_MIN - SCENE_MAX + ALIENHOME + 0.05f)));
    DOUBLES_EQUAL(3.6f, a.roty,FLOAT_PRECISION);

    mock().expectNCalls(2,"gluDeleteQuadric");
}

//TODO: more alien display and animation tests
//TODO: explozans

TEST(ModelTestGroup, ShotIsWhatUC){
    mock().expectNCalls(1,"gluNewQuadric");
    auto gm = Game();
    auto s = Shot(gm, Point3f(ONE, ONE, ONE));

    mock().expectNCalls(2,"glPushMatrix");
    mock().expectNCalls(2,"glPopMatrix");
    mock().expectNCalls(1,"glTranslatef");
    mock().expectOneCall("glScalef");
    mock().expectOneCall("glColor3f");
    mock().expectNCalls(1,"gluSphere");

    mock().expectOneCall("Game::getBrickAt");
    s.animate(0.01f).display();
    mock().checkExpectations();
    DOUBLES_EQUAL( 0.6, s.place.z, FLOAT_PRECISION);

    mock().expectNCalls(1,"gluDeleteQuadric");
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
    CHECK(Point3i(0,60,90).eq(*pos));

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

    got = Brick::getBrick(iss, &game);
    CHECK(got.has_value());
    CHECK(got.value().place.eq(Point3i(0,40,60)));
    CHECK(got.value().rgb.eq(Point3f(SILVER)));
    CHECK(got.value().type == BRIK_SILVER);

    got = Brick::getBrick(iss, &game);
    CHECK(got.has_value());
    CHECK(got.value().place.eq(Point3i(10,0,60)));
    CHECK(got.value().rgb.eq(Point3f(GOLD)));
    CHECK(got.value().type == BRIK_GOLDEN);

    got = Brick::getBrick(iss, &game);
    CHECK(got.has_value());
    CHECK(got.value().place.eq(Point3i(10,1,60)));
    CHECK(got.value().rgb.eq(Point3f(0.1,0,0)));
    CHECK(got.value().type == BRIK_NORMAL);
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
