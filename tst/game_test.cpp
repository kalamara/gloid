#include "GLoid.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "game/Loading.h"

//C++ libraries

void srand(unsigned int s) {
    mock().actualCall("srand");
}

std::unique_ptr<Game> newGame(){
   // mock().expectOneCall("Loading::Loading");
    mock().expectOneCall("SDL_WM_SetCaption");
    mock().expectOneCall("SDL_Init");
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

    mock().expectOneCall("SDL_PollEvent").andReturnValue(1);
    game->loop();

    game->app.visible = false;
//app is not visible, next loop should block on SDL_WaitEvent
    mock().expectOneCall("SDL_PollEvent").andReturnValue(0);
    mock().expectOneCall("SDL_WaitEvent");
    game->loop();
    mock().checkExpectations();
//on visible, run next step update and draw
    game->app.visible = true;
    mock().expectOneCall("SDL_PollEvent").andReturnValue(0);
    //from update
    mock().expectOneCall("TTF_RenderText_Shaded");
    mock().expectNCalls(11,"SDL_RWFromFile");
    mock().expectNCalls(11,"SDL_LoadWAV_RW");
    mock().expectNCalls(11,"SDL_GetError");
    //from draw
    mock().expectOneCall("SDL_GL_SwapBuffers");
//first step should be loading
    game->loop();
    CHECK_EQUAL(STEP_LOADING, game->queryStep());
    mock().checkExpectations();
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
