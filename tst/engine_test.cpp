#include "GLoid.h"
#include <sstream>

#include <vector>
#include <map>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "game/Loading.h"

//C++ libraries

void srand(unsigned int s) {
    mock().actualCall("srand");
}


Loading::Loading(Game *g){

}

Loading::~Loading(){

}

Loading * Loading::draw(){

}

Loading * Loading::update(){

}

Loading * Loading::next(){

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

TEST(GameTestGroup, pow_test){
    CHECK_EQUAL(0, Engine<Game>::nextpoweroftwo(0));
    CHECK_EQUAL(1, Engine<Game>::nextpoweroftwo(1));
    CHECK_EQUAL(2, Engine<Game>::nextpoweroftwo(2));
    CHECK_EQUAL(4, Engine<Game>::nextpoweroftwo(3));
    CHECK_EQUAL(256, Engine<Game>::nextpoweroftwo(129));
    CHECK_EQUAL(256, Engine<Game>::nextpoweroftwo(256));

}

TEST(GameTestGroup, text_test){
    std::stringstream str;
    text2d::log(&str, "ena =");

    STRCMP_EQUAL("ena =\n",str.str().c_str());

    str.str( std::string() );
    str.clear();
    text2d::log(&str, "ena = ", 1, ",~dyo ison ", 2.00001f);

    STRCMP_EQUAL("ena = 1,~dyo ison 2.00001\n", str.str().c_str());

    text2d * text = new text2d();
    text = text->print("ena =");
    STRCMP_EQUAL("ena =\n", text->msg().c_str());

    text = text->clear()->print("ena = ", 1, ",~dyo ison ", 2.00001f);

    STRCMP_EQUAL("ena = 1,~dyo ison 2.00001\n",  text->msg().c_str());

    mock().expectOneCall("TTF_RenderText_Shaded");
    Game * game = newGame();
    SDL_Surface * s = game->print2d(*text);

    game->draw2d(s,0,0);
    delete text;
    delete game;

}

TEST(GameTestGroup, time_test){
    Game * game = newGame();
    mock().expectOneCall("SDL_GetTicks");
    game->toc();
    mock().checkExpectations();
    delete game;
}

TEST(GameTestGroup, version_test){
    CHECK_EQUAL(0, version().value());
    STRCMP_EQUAL("V0.0.0", version().toString().c_str());

    unsigned int ver = 0x010203;
    CHECK_EQUAL(1, version(ver).major);
    CHECK_EQUAL(2, version(ver).minor);
    CHECK_EQUAL(3, version(ver).patch);

    CHECK_EQUAL(ver, version(ver).value());
    CHECK_EQUAL(ver, version(1,2,3).value());

    STRCMP_EQUAL("V1.2.3", version(ver).toString().c_str());
}

TEST(GameTestGroup, init_test){
    mock().expectOneCall("SDL_Init");
    mock().expectOneCall("SDL_WM_SetCaption");
    mock().expectOneCall("SDL_GetTicks").andReturnValue(123);
    mock().expectOneCall("srand");

    Game * game = new Game();

    mock().checkExpectations();
//old sdl library -> go hunting for desktop setup
    struct version sdlv = version(1,2,8);

    mock().expectNCalls(36, "SDL_SetVideoMode");
    mock().expectOneCall("SDL_GetError");

    game = game->withSdlGlVideo(sdlv);

    mock().checkExpectations();

//newer sdl library -> call sdlgetvideoinfo
    sdlv = version(1,2,9);

    mock().expectOneCall("SDL_GetVideoInfo");
    mock().expectNCalls(3, "SDL_SetVideoMode");
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game = game->withSdlGlVideo(sdlv);
    mock().checkExpectations();

/*ttf font*/
/* ttf_init fails*/
    mock().expectOneCall("TTF_Init").andReturnValue(-1);
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game = game->withSdlTtf("./DejaVuSans.ttf");

    CHECK(game->getFont() == nullptr);
    mock().checkExpectations();
/*ttf_init succeeds, ttf_openfont fails*/
    mock().expectOneCall("TTF_Init").andReturnValue(0);
    mock().expectOneCall("TTF_OpenFont");
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game = game->withSdlTtf("./DejaVuSans.ttf");
    CHECK(game->getFont() == nullptr);
    mock().checkExpectations();
/*sdl audio*/
/*openaudio fails*/
    mock().expectOneCall("SDL_OpenAudio").andReturnValue(-1);
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game = game->withSdlAudio(22050, 2, 0);

    mock().checkExpectations();
    CHECK(game->getSdlAudio() == nullptr);

/*openaudio succeds*/
    mock().expectOneCall("SDL_OpenAudio").andReturnValue(0);
    mock().expectOneCall("SDL_PauseAudio");

    game = game->withSdlAudio(22050, 2, 512);

    mock().checkExpectations();
    CHECK(game->getSdlAudio() != nullptr);

/*OpenGL*/
    mock().expectOneCall("glClearColor");
    mock().expectOneCall("glClearDepth");
    mock().expectOneCall("glDepthFunc");
    mock().expectOneCall("glShadeModel");
    mock().expectNCalls(2,"glHint");
    mock().expectOneCall("glBlendFunc");
    mock().expectNCalls(3,"glEnable");
    mock().expectNCalls(2,"glLightfv");
    mock().expectNCalls(5,"glMaterialfv");
    mock().expectOneCall("glViewport");
    mock().expectNCalls(2,"glLoadIdentity");
    mock().expectNCalls(2,"glMatrixMode");
    mock().expectOneCall("gluPerspective");
    mock().expectOneCall("gluLookAt");
    mock().expectOneCall("glClear");

    game = game->withOpenGl();
    mock().checkExpectations();

    delete game;
}

TEST(GameTestGroup, sound_test){
    Game::mixer(nullptr, nullptr, 0);
    unsigned char mix[6] = {0};
    unsigned char data1[6] = "12345";
    unsigned char data2[7] = "678910";

    std::vector<struct sbuffer> buffers;//(NUM_BUFFERS);

    buffers.push_back(sbuffer(data1, 6));
    buffers.push_back(sbuffer(data2, 7));

    mock().expectNCalls(2, "SDL_MixAudio");

    Game::mixer((void *)&buffers, mix, 2);
    CHECK_EQUAL(4,buffers[0].dlen);
    CHECK_EQUAL(5,buffers[1].dlen);
    STRCMP_EQUAL("345", (const char *)buffers[0].data);
    STRCMP_EQUAL("8910",(const char *)buffers[1].data);

    mock().expectNCalls(2, "SDL_MixAudio");

    Game::mixer((void *)&buffers, mix, 5);
    CHECK_EQUAL(0,buffers[0].dlen);
    CHECK_EQUAL(0,buffers[1].dlen);
    STRCMP_EQUAL("", (const char *)buffers[0].data);
    STRCMP_EQUAL("", (const char *)buffers[1].data);
    mock().checkExpectations();

    /*we need an instance of game to actually play a sound*/

    Game * game = newGame();

    mock().expectOneCall("SDL_OpenAudio").andReturnValue(0);
    mock().expectOneCall("SDL_PauseAudio");

    game = game->withSdlAudio(22050, 2, 512);

    mock().checkExpectations();

    //newly alloc'd data by SDL_LoadWAV
    unsigned char  loaded[8] = "1234567";
    mock().expectOneCall("SDL_BuildAudioCVT");
    mock().expectOneCall("SDL_ConvertAudio");
    game = game->addSound(loaded, 8, "alien");

    bool succ = game->playSound("");
    CHECK_EQUAL(0, game->pendingSounds());
    CHECK(!succ);

    succ = game->playSound("alien");
    CHECK(succ);
    CHECK_EQUAL(1, game->pendingSounds());

    delete game;
}

TEST(GameTestGroup, events_test){
    extern unsigned char MockKeyboard[SDLK_LAST];
    Game * game = newGame();
    SDL_Event evt;
    evt.type = SDL_QUIT;

    game = game->handleEvent(evt);

    CHECK(!game->looping());

    evt.type = SDL_ACTIVEEVENT;
    evt.active.state = SDL_APPACTIVE | SDL_APPMOUSEFOCUS | SDL_APPINPUTFOCUS;
    evt.active.gain = false;

    game = game->handleEvent(evt);

    CHECK(!game->visible());
    CHECK(!game->mouseFocusing());
    CHECK(!game->kbdFocusing());

    evt.active.gain = true;

    game = game->handleEvent(evt);

    CHECK(game->visible());
    CHECK(game->mouseFocusing());
    CHECK(game->kbdFocusing());

    evt.type = SDL_VIDEORESIZE;
    evt.resize.w = 800;
    evt.resize.h = 600;
    mock().expectOneCall("glViewport");
    mock().expectNCalls(2,"glLoadIdentity");
    mock().expectNCalls(2,"glMatrixMode");
    mock().expectOneCall("gluPerspective");
    mock().expectOneCall("gluLookAt");

    game = game->handleEvent(evt);

    evt.type = SDL_KEYDOWN;

    MockKeyboard[SDLK_RETURN] = true;
    mock().expectOneCall("SDL_GetKeyState").andReturnValue(MockKeyboard);

    game = game->handleEvent(evt);

    CHECK(game->keyPressed(SDLK_RETURN));

    evt.type = SDL_MOUSEMOTION;
    evt.motion.x = 15;
    evt.motion.y = 20;

    game = game->handleEvent(evt);

    CHECK_EQUAL(15, game->getMouse()->X);
    CHECK_EQUAL(20, game->getMouse()->Y);

    evt.type = SDL_MOUSEBUTTONDOWN;

    game = game->handleEvent(evt);

    CHECK(game->getMouse()->leftclick);

    evt.type = SDL_MOUSEBUTTONUP;

    game = game->handleEvent(evt);

    CHECK(!game->getMouse()->leftclick);

    delete game;
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
