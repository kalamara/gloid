#include "GLoid.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "game/Loading.h"

//C++ libraries

void srand(unsigned int s) {
    mock().actualCall("srand");
}

Game * newGame(){
   // mock().expectOneCall("Loading::Loading");
    mock().expectOneCall("SDL_WM_SetCaption");
    mock().expectOneCall("SDL_Init");
    mock().expectOneCall("SDL_GetTicks")
            .andReturnValue(123);
    mock().expectOneCall("srand");
    return new Game();
}

TEST_GROUP(GameTestGroup){
    void teardown(){
            mock().clear();
    }
};
/*
TEST(GameTestGroup, step_factory_test){
    Game * game = newGame();
    auto s = game->nextStep();
    CHECK_EQUAL(STEP_LOADING, s->type);

    delete game;
    //delete s;
}
*/
TEST(GameTestGroup, loop_test){

    Game * game = newGame();

    mock().expectOneCall("SDL_PollEvent").andReturnValue(1);
    game = game->loop();

    game->app.visible = false;
//app is not visible, next loop should block on SDL_WaitEvent
    mock().expectOneCall("SDL_PollEvent").andReturnValue(0);
    mock().expectOneCall("SDL_WaitEvent");
    game = game->loop();
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
    game = game->loop();
    CHECK_EQUAL(STEP_LOADING, game->queryStep());
    mock().checkExpectations();

    delete game;
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
