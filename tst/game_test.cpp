#include "GLoid.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "game/Loading.h"
#include "model/Brick.h"
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

    auto loadStep = dynamic_cast<Loading *>(&(game->nextStep()));
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
}

//parsing levels
TEST(GameTestGroup, parsing_test){

    auto game = newGame();
    mock().checkExpectations();
    Loading * load = new Loading(*game);
    auto pos = load->getPos(std::string());
    CHECK(!pos);
    pos = load->getPos("");
    CHECK(!pos);
    pos = load->getPos("lol");
    CHECK(!pos);
    pos = load->getPos("TM_POS ");
    CHECK(!pos);
    pos = load->getPos("TM_POST ");
    CHECK(!pos);
    pos = load->getPos("TM_POST ROCK");
    CHECK(!pos);
    pos = load->getPos("TM_POS\tl");
    CHECK(!pos);
    pos = load->getPos("TM_POS\tl\t0\tl");
    CHECK(!pos);
    pos = load->getPos("TM_POS\t1.0\t20.0 ");
    CHECK(!pos);

    pos = load->getPos("TM_POS  0.0	60.0	90.0");
    CHECK(Point3i(0,60,90).eq(*pos));

    auto col = load->getColor("WIREFRAME_COLOR  0.3	 0.3	 0.3");
    CHECK(Point3f(0.3,0.3,0.3).eq(*col));

    auto tokens = load->getTokens("*GEOMOBJECT {\
                                      *TM_POS  0.0	 0.0	60.0\
                                  *WIREFRAME_COLOR  0.3	 0.3	 0.3\
                              *GEOMOBJECT {\
                                      *TM_POS 10.0	 0.0	60.0\
                                  *WIREFRAME_COLOR  0.3	 0.3	 0.3"
                                      , {});
    STRCMP_CONTAINS("GEOMOBJECT", tokens[0].c_str());
    STRCMP_CONTAINS("TM_POS", tokens[1].c_str());
    STRCMP_CONTAINS("WIREFRAME_COLOR", tokens[2].c_str());

    delete load;
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
