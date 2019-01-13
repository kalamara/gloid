#include "GLoid.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "game/Loading.h"
#include "game/Waiting.h"

#include "model/Alien.h"
#include "model/Ball.h"
#include "model/Brick.h"
#include "model/Particle.h"
#include "model/Pill.h"
#include "model/Vaus.h"

//C++ libraries
// Bitmaps

enum{
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
};

// Sounds
enum{
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
};

void srand(unsigned int s) {
    mock().actualCall("srand");
}

std::unique_ptr<Game> newGame(){
    // mock().expectOneCall("Loading::Loading");
    mock().expectOneCall("SDL_WM_SetCaption");
    mock().expectOneCall("SDL_Init");
    mock().expectOneCall("SDL_GetTicks")
            .andReturnValue(123);
    mock().expectOneCall("SDL_GetTicks")
            .andReturnValue(123);
    mock().expectOneCall("SDL_GetTicks")
            .andReturnValue(123);
    mock().expectOneCall("srand");
    return std::make_unique<Game>();
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
    mock().checkExpectations();

    mock().expectOneCall("SDL_PollEvent").andReturnValue(1);
    game->loop();

    game->app.visible = false;
    //app is not visible, next loop should block on SDL_WaitEvent
    mock().expectOneCall("SDL_PollEvent").andReturnValue(0);
    mock().expectOneCall("SDL_WaitEvent");
    game->loop();
    mock().checkExpectations();
    mock().expectNCalls(N_WAV,"SDL_GetTicks");
    mock().expectNCalls(1,"SDL_GetTicks");
    //on visible, run next step update and draw
    game->app.visible = true;
    mock().expectOneCall("SDL_PollEvent").andReturnValue(0);
    //from update
    //font is not initialized yet
    //mock().expectOneCall("TTF_RenderText_Shaded");
    //audio is not initialized yet
    //mock().expectNCalls(11,"SDL_RWFromFile");
    //mock().expectNCalls(11,"SDL_LoadWAV_RW");

    mock().expectNCalls(N_WAV,"SDL_GetError");
    //from draw
    mock().expectOneCall("SDL_GL_SwapBuffers");
    //first step should be loading
    game->loop();
    CHECK_EQUAL(STEP_LOADING, game->queryStep());
    mock().checkExpectations();
}

TEST(GameTestGroup, loading_test){
    auto game = newGame();
    mock().checkExpectations();

    auto loadStep = dynamic_cast<Loading *>(game->nextStep());
    CHECK_EQUAL(STEP_LOADING,loadStep->type);
    CHECK_EQUAL(LOAD_SOUNDS,loadStep->phase);
    mock().expectNCalls(N_WAV,"SDL_GetError");
    mock().expectNCalls(N_WAV,"SDL_GetTicks");
    mock().expectNCalls(1,"SDL_GetTicks");
    loadStep->update();

    mock().checkExpectations();
    CHECK_EQUAL(LOAD_TEXTURES,loadStep->phase);

    mock().expectOneCall("glGenTextures");
    mock().expectNCalls(N_BMP,"SDL_LoadBMP_RW");
    mock().expectNCalls(N_BMP,"SDL_RWFromFile");
//    mock().expectNCalls(N_BMP,"SDL_SetColorKey");
//    mock().expectNCalls(N_BMP,"SDL_MapRGBA");
    mock().expectNCalls(N_BMP,"SDL_GetError");
    mock().expectNCalls(N_BMP,"SDL_GetTicks");
    mock().expectNCalls(1,"SDL_GetTicks");
    loadStep->update();
//should have allocated textureIds
    CHECK_EQUAL(N_BMP, loadStep->textureIds[N_BMP-1]);
    mock().checkExpectations();
    CHECK_EQUAL(LOAD_HALLOFAME, loadStep->phase);

    mock().expectNCalls(1,"SDL_GetTicks");
    loadStep->update();
    CHECK_EQUAL(10, game->hiscore.size());
    CHECK_EQUAL(50000,game->hiscore.begin()->first);
    STRCMP_EQUAL("AAA",game->hiscore.begin()->second.c_str());
    CHECK_EQUAL( 50000,std::prev(game->hiscore.end())->first);
    STRCMP_EQUAL("AAA",std::prev(game->hiscore.end())->second.c_str());
    mock().checkExpectations();
    CHECK_EQUAL(LOAD_LEVEL, loadStep->phase);
    mock().expectNCalls(1,"SDL_GetTicks");
    loadStep->update();
    mock().checkExpectations();
    CHECK_EQUAL(N_LOAD, loadStep->phase);

    //should not change any more

    loadStep->update();
    mock().checkExpectations();
    CHECK_EQUAL(N_LOAD, loadStep->phase);
    CHECK_EQUAL(STEP_WAITING, loadStep->next());
}

TEST(GameTestGroup, waiting_test){
    auto game = newGame();
    mock().checkExpectations();

    auto waitStep = new Waiting(*game);
    CHECK_EQUAL(STEP_WAITING,waitStep->type);
    delete waitStep;
}

//parsing scores
TEST(GameTestGroup, score_test){
    auto game = newGame();
    mock().checkExpectations();
    Loading * load = new Loading(*game);
    auto score = load->getScore(std::string());
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load->getScore("");
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load->getScore("lol");
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load->getScore("lol ");
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load->getScore("lol lol");
    CHECK(!score.second.size());
    CHECK(!score.first);
    score = load->getScore("lol 123");
    STRCMP_EQUAL("LOL", score.second.c_str());
    CHECK_EQUAL(123, score.first);

    score = load->getScore("lola 12345678.999");
    STRCMP_EQUAL("LOL", score.second.c_str());
    CHECK_EQUAL(12345678, score.first);

    delete load;
}


int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
