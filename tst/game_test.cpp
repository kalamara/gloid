#include "GLoid.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "Hud.h"
#include "game/Loading.h"
#include "game/Waiting.h"
#include "game/Intro.h"
#include "game/Play.h"

#include "model/Alien.h"
#include "model/Ball.h"
#include "model/Brick.h"
#include "model/Particle.h"
#include "model/Pill.h"
#include "model/Vaus.h"

// Bitmaps
typedef enum{
   BMP_BG_1,
   BMP_BG_2,
   BMP_BG_3,
   BMP_BG_4,
   BMP_LOGO,
   BMP_VAUS,
   BMP_TARGET,
   BMP_CROSSHAIRS,
   BMP_WARP,
   N_BMP
}BMP_IDS;

// Sounds
typedef enum{
   WAV_ALIEN,
   WAV_BOUNCE0,
   WAV_BOUNCE1,
   WAV_ENLARGE,
   WAV_GO,
   WAV_INTRO,
   WAV_KANONAKI,
   WAV_LAUNCH,
   WAV_LOSE,
   WAV_SHOT,
   WAV_WARP,
   N_WAV
}WAV_IDS;

//C++ libraries

void srand(unsigned int s) {
    mock().actualCall("srand");
}

std::unique_ptr<class Game> newGame(){

    mock().expectOneCall("SDL_WM_SetCaption");
    mock().expectOneCall("SDL_Init");
    mock().expectOneCall("SDL_GetTicks")
            .andReturnValue(123);
    mock().expectOneCall("SDL_GetTicks")
            .andReturnValue(123);
    mock().expectOneCall("SDL_GetTicks")
            .andReturnValue(123);
    mock().expectOneCall("srand");
    auto g = std::make_unique<class Game>();
    mock().checkExpectations();
    return g;
}

Alien::Alien(Game & g){}
Alien::~Alien(){}

void Alien::display(void){}
Alien& Alien::animate(double secPerFrame){}

Ball::Ball(class Game &g){}
Ball::~Ball(){}

void Ball::display(void){}
Ball& Ball::animate(double secPerFrame){}

Brick::Brick(class Game &g,
             const Point3f& color,
             const Point3i& coords,
             int t){}
Brick::~Brick(){}

void Brick::display(void){}
Brick& Brick::animate(double secPerFrame){}
std::optional<Brick> Brick::getBrick(std::istream & ifs, Game * game){
    return {};
}

Particle::Particle(const Point3f & where,
               const Point3f & color,
               float len){}
Particle::~Particle(){}

void Particle::display(void){}
Particle& Particle::animate(double secPerFrame){}

Pill::Pill(const Point3f& where, Game &g){}
Pill::~Pill(){}

void Pill::display(void){}
Pill& Pill::animate(double secPerFrame){}

bool Pill::collides(float left,
              float right,
              float up,
              float down){}

Vaus::Vaus(class Game &g){}
Vaus::~Vaus(){}

void Vaus::display(void){}
Vaus& Vaus::animate(double secPerFrame){}

TEST_GROUP(GameTestGroup){
    void teardown(){
        mock().clear();
    }
};

TEST(GameTestGroup, loop_test){

    auto game = newGame();

    mock().expectOneCall("SDL_PollEvent").andReturnValue(1);
    game->loop();

    game->app.visible = false;

    //app is not visible, next loop should block on SDL_WaitEvent
    mock().expectOneCall("SDL_PollEvent").andReturnValue(0);
    mock().expectOneCall("SDL_WaitEvent");
    game->loop();
    mock().checkExpectations();
    mock().expectNCalls(N_WAV,"SDL_GetTicks");
#if(LOGLEVEL < LOG_DEBUG + 1)  //one additional call to getticks from logging
    mock().expectNCalls(1,"SDL_GetTicks");
#endif
    //on visible, run next step update and draw
    game->app.visible = true;

    mock().expectOneCall("SDL_PollEvent").andReturnValue(0);

    //font is not initialized yet
    //audio is not initialized yet

    mock().expectNCalls(N_WAV,"SDL_GetError");
    //from draw
    mock().expectNCalls(2,"SDL_GL_SwapBuffers");
    mock().expectNCalls(2,"glClear");
    //first step should be loading
    game->loop();
    CHECK_EQUAL(STEP_LOADING, game->queryStep());
    mock().checkExpectations();
}

TEST(GameTestGroup, hud_test){

    auto text = TextBody();

    auto game = newGame();

    game->setFont((TTF_Font *)nullptr);
    mock().checkExpectations();

    auto hud = Hud();
//no game should not crash
    hud.printText("lol", text, White, 0);
    hud.drawText(text);
    hud.setGame(*game);

//text body should be preallocated
//if no line defined, push back
    mock().expectOneCall("TTF_RenderText_Shaded");
    hud.printText("lol",text, White, 0);
    mock().checkExpectations();
    CHECK_EQUAL(1, text.body.size());
//if same line, replace
    mock().expectOneCall("SDL_FreeSurface");
    mock().expectOneCall("TTF_RenderText_Shaded");
    hud.printText("lawl",text, White, 0);
    mock().checkExpectations();
    CHECK_EQUAL(1, text.body.size());
//if huge line defined, push back
    mock().expectOneCall("TTF_RenderText_Shaded");
    hud.printText("lalala",text, White, 1234);
    mock().checkExpectations();
    CHECK_EQUAL(2, text.body.size());
}

TEST(GameTestGroup, loading_test){
    auto game = newGame();
    mock().checkExpectations();

    auto loadStep = dynamic_cast<Loading *>(game->nextStep());
    CHECK_EQUAL(STEP_LOADING,loadStep->type);
    CHECK_EQUAL(LOAD_SOUNDS,loadStep->phase);
    mock().expectNCalls(N_WAV,"SDL_GetError");
    mock().expectNCalls(N_WAV,"SDL_GetTicks");
#if(LOGLEVEL < LOG_DEBUG + 1)  //one additional call to getticks from logging
    mock().expectNCalls(1,"SDL_GetTicks");
#endif
    loadStep->update();

    mock().checkExpectations();
    CHECK_EQUAL(LOAD_TEXTURES,loadStep->phase);

    mock().expectOneCall("glGenTextures");
    mock().expectNCalls(N_BMP,"SDL_LoadBMP_RW");
    mock().expectNCalls(N_BMP,"SDL_RWFromFile");

    mock().expectNCalls(N_BMP,"SDL_GetError");
    mock().expectNCalls(N_BMP,"SDL_GetTicks");
#if(LOGLEVEL < LOG_DEBUG + 1)  //one additional call to getticks from logging
    mock().expectNCalls(1,"SDL_GetTicks");
#endif
    loadStep->update();
//should have allocated textureIds for all textures + 1 for text
    CHECK_EQUAL(N_BMP + 1, loadStep->textureIds[N_BMP]);
    mock().checkExpectations();
    CHECK_EQUAL(LOAD_HALLOFAME, loadStep->phase);
#if(LOGLEVEL < LOG_DEBUG + 1)  //one additional call to getticks from logging
    mock().expectNCalls(1,"SDL_GetTicks");
#endif
    loadStep->update();
    CHECK_EQUAL(10, game->hiscore.size());
    CHECK_EQUAL(50000,game->hiscore.begin()->first);
    STRCMP_EQUAL("AAA",game->hiscore.begin()->second.c_str());
    CHECK_EQUAL( 50000,std::prev(game->hiscore.end())->first);
    STRCMP_EQUAL("AAA",std::prev(game->hiscore.end())->second.c_str());
    mock().checkExpectations();
    CHECK_EQUAL(LOAD_LEVEL, loadStep->phase);
#if(LOGLEVEL < LOG_DEBUG + 1)  //one additional call to getticks from logging
    mock().expectNCalls(1,"SDL_GetTicks");
#endif
    loadStep->update();
    mock().checkExpectations();
    CHECK_EQUAL(LOAD_DONE, loadStep->phase);

    //should not change any more

    loadStep->update();
    mock().checkExpectations();
    CHECK_EQUAL(LOAD_DONE, loadStep->phase);
  ///  CHECK_EQUAL(STEP_WAITING, loadStep->next());
    mock().checkExpectations();
}

//parsing scores
TEST(GameTestGroup, score_test){
    auto game = newGame();
    mock().checkExpectations();
    Loading load = Loading(*game);
    auto score = load.getScore(std::string());
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load.getScore("");
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load.getScore("lol");
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load.getScore("lol ");
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load.getScore("lol lol");
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load.getScore("lol 123");
    STRCMP_EQUAL("LOL", score.second.c_str());
    CHECK_EQUAL(123, score.first);

    score = load.getScore("lola 12345678.999");
    STRCMP_EQUAL("LOL", score.second.c_str());
    CHECK_EQUAL(12345678, score.first);
}


TEST(GameTestGroup, waiting_test){
    auto game = newGame();
    mock().checkExpectations();

    auto waitStep = Waiting(*game);
    CHECK_EQUAL(STEP_WAITING,waitStep.type);

    CHECK_EQUAL(WAIT_RDY,waitStep.flip(0));
    CHECK_EQUAL(WAIT_RDY,waitStep.flip(1));
    CHECK_EQUAL(WAIT_HOF,waitStep.flip(FLIP_PHASE));
    CHECK_EQUAL(WAIT_HOF,waitStep.flip(FLIP_PHASE + 1));
    CHECK_EQUAL(WAIT_RDY,waitStep.flip(2 * FLIP_PHASE));
    CHECK_EQUAL(WAIT_RDY,waitStep.flip(2 * FLIP_PHASE + 1));
    CHECK_EQUAL(WAIT_HOF,waitStep.flip(3 * FLIP_PHASE));
}

TEST(GameTestGroup, level_test){
    auto game = newGame();
    mock().checkExpectations();
    game->level = 1;
    STRCMP_EQUAL("arka1_hires", game->getBackgroundTextureFilename().c_str());

    game->level = 4;
    STRCMP_EQUAL("arka4_hires", game->getBackgroundTextureFilename().c_str());

    game->level = 5;
    STRCMP_EQUAL("arka1_hires", game->getBackgroundTextureFilename().c_str());

}

TEST(GameTestGroup, intro_test){
    auto game = newGame();
    mock().checkExpectations();
    auto introStep = Intro(*game);
    CHECK_EQUAL(STEP_INTRO,introStep.type);

    std::pair<unsigned int, std::string> newLine = {};

    newLine = introStep.readLine(0);

    CHECK_EQUAL(0,newLine.first);
    CHECK_EQUAL(0,newLine.second.size());

    newLine = introStep.readLine(1); //read up to line 0 char 1 (excl.)

    CHECK_EQUAL(0,newLine.first);
    STRCMP_EQUAL("T", newLine.second.c_str());

    newLine = introStep.readLine(20);

    CHECK_EQUAL(0,newLine.first);
    STRCMP_EQUAL("THE ERA AND TIME OF ", newLine.second.c_str());

    newLine = introStep.readLine(21);

    CHECK_EQUAL(1,newLine.first);
    STRCMP_EQUAL("T", newLine.second.c_str());


    newLine = introStep.readLine(0xFFFF);

    CHECK_EQUAL(8,newLine.first);
    STRCMP_EQUAL("BY SOMEONE...", newLine.second.c_str());

}

int main(int ac, char** av){
    return CommandLineTestRunner::RunAllTests(ac, av);
}
