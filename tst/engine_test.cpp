
#define LOGLEVEL LOG_WARNING
#include "GLoid.h"
#include <sstream>

#include <vector>
#include <map>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "game/Loading.h"
#include "mock_sdl_opengl.h"

TEST_GROUP(EngineTestGroup){
    void teardown(){
            mock().clear();
    }
};

TEST(EngineTestGroup, pow_test){
    CHECK_EQUAL(0, Engine<Game>::log2n(0));
    CHECK_EQUAL(0, Engine<Game>::log2n(1));
    CHECK_EQUAL(1, Engine<Game>::log2n(2));
    CHECK_EQUAL(7, Engine<Game>::log2n(129));

    CHECK_EQUAL(0, Engine<Game>::nextpoweroftwo(0));
    CHECK_EQUAL(1, Engine<Game>::nextpoweroftwo(1));
    CHECK_EQUAL(2, Engine<Game>::nextpoweroftwo(2));
    CHECK_EQUAL(4, Engine<Game>::nextpoweroftwo(3));
    CHECK_EQUAL(256, Engine<Game>::nextpoweroftwo(129));
    CHECK_EQUAL(256, Engine<Game>::nextpoweroftwo(256));

    CHECK(Engine<Game>::ispoweroftwo(0));
    CHECK(Engine<Game>::ispoweroftwo(1));
    CHECK(Engine<Game>::ispoweroftwo(2));
    CHECK(Engine<Game>::ispoweroftwo(4));
    CHECK(Engine<Game>::ispoweroftwo(8));
    CHECK(Engine<Game>::ispoweroftwo(16));
    CHECK(Engine<Game>::ispoweroftwo(64));
    CHECK(Engine<Game>::ispoweroftwo(256));
    CHECK(Engine<Game>::ispoweroftwo(1024));
    CHECK(Engine<Game>::ispoweroftwo(4096));

    CHECK_FALSE(Engine<Game>::ispoweroftwo(3));
    CHECK_FALSE(Engine<Game>::ispoweroftwo(4098));
}

TEST(EngineTestGroup, text_test){
    std::stringstream str;
    text2d::log(&str, "ena =");

    STRCMP_EQUAL("ena =\n",str.str().c_str());

    str.str( std::string() );
    str.clear();
    mock().expectNCalls(2, "SDL_GetTicks");
    text2d::log(&str, "ena = ", 1, ",~dyo ison ", 2.00001f);


    STRCMP_EQUAL("ena = 1,~dyo ison 2.00001\n", str.str().c_str());

    text2d text = text2d();
    text.print("ena =");
    STRCMP_EQUAL("ena =\n", text.msg().c_str());

    STRCMP_EQUAL("ena =", text.trim().c_str());

    text.clear()->print("ena = ", 1, ",~dyo ison ", 2.00001f);

    STRCMP_EQUAL("ena = 1,~dyo ison 2.00001\n",  text.msg().c_str());

    mock().expectOneCall("TTF_RenderText_Shaded");
    auto game = newGame();
    SDL_Surface * s = game->print2d(text);

    game->draw2d(s,0,0);
}

TEST(EngineTestGroup, time_test){
    auto game = newGame();
    mock().expectOneCall("SDL_GetTicks");
    game->toc();
    mock().checkExpectations();
}

TEST(EngineTestGroup, version_test){
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

TEST(EngineTestGroup, init_test){

    auto game = newGame();
    mock().checkExpectations();
//old sdl library -> go hunting for desktop setup
    struct version sdlv = version(1,2,8);
    mock().expectNCalls(2,"SDL_GetTicks");
    mock().expectNCalls(36, "SDL_SetVideoMode");
    mock().expectOneCall("SDL_GetError");

    game->withSdlGlVideo(sdlv);

    mock().checkExpectations();

//newer sdl library -> call sdlgetvideoinfo
    sdlv = version(1,2,9);
    mock().expectNCalls(1,"SDL_GetTicks");
    mock().expectOneCall("SDL_GetVideoInfo");
    mock().expectNCalls(3, "SDL_SetVideoMode");
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game->withSdlGlVideo(sdlv);
    mock().checkExpectations();

/*ttf font*/
/* ttf_init fails*/
    mock().expectNCalls(1,"SDL_GetTicks");
    mock().expectOneCall("TTF_Init").andReturnValue(-1);
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game->withSdlTtf("./DejaVuSans.ttf");

    CHECK(!game->getFont());
    mock().checkExpectations();
/*ttf_init succeeds, ttf_openfont fails*/
    mock().expectNCalls(1,"SDL_GetTicks");
    mock().expectOneCall("TTF_Init").andReturnValue(0);
    mock().expectOneCall("TTF_OpenFont");
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game->withSdlTtf("./DejaVuSans.ttf");
    CHECK(!game->getFont());
    mock().checkExpectations();
/*sdl audio*/
/*openaudio fails*/
    mock().expectNCalls(1,"SDL_GetTicks");
    mock().expectOneCall("SDL_OpenAudio").andReturnValue(-1);
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game->withSdlAudio(22050, 2, 0);

    mock().checkExpectations();
    CHECK(!game->getSdlAudio());

/*openaudio succeds*/
    mock().expectNCalls(1,"SDL_GetTicks");
    mock().expectOneCall("SDL_OpenAudio").andReturnValue(0);
    mock().expectOneCall("SDL_PauseAudio");

    game->withSdlAudio(22050, 2, 512);

    mock().checkExpectations();
    CHECK(game->getSdlAudio().has_value());

/*OpenGL*/
    mock().expectNCalls(2,"SDL_GetTicks");
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

    game->withOpenGl();
    mock().checkExpectations();
}

TEST(EngineTestGroup, sound_test){
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

    auto game = newGame();
    mock().expectNCalls(1,"SDL_GetTicks");
    mock().expectOneCall("SDL_OpenAudio").andReturnValue(0);
    mock().expectOneCall("SDL_PauseAudio");

    game->withSdlAudio(22050, 2, 512);

    mock().checkExpectations();

    //newly alloc'd data by SDL_LoadWAV
    unsigned char  loaded[8] = "1234567";
    mock().expectOneCall("SDL_BuildAudioCVT");
    mock().expectOneCall("SDL_ConvertAudio");
    game->addSound(loaded, 8, "alien");

    bool succ = game->playSound("");
    CHECK_EQUAL(0, game->pendingSounds());
    CHECK(!succ);

    succ = game->playSound("alien");
    CHECK(succ);
    CHECK_EQUAL(1, game->pendingSounds());
}

TEST(EngineTestGroup, events_test){
    extern unsigned char MockKeyboard[SDLK_LAST];
    auto game = newGame();
    SDL_Event evt;
    evt.type = SDL_QUIT;
    mock().expectNCalls(2,"SDL_GetTicks");
    game->handleEvent(evt);

    CHECK(!game->looping());

    evt.type = SDL_ACTIVEEVENT;
    evt.active.state = SDL_APPACTIVE | SDL_APPMOUSEFOCUS | SDL_APPINPUTFOCUS;
    evt.active.gain = false;

    game->handleEvent(evt);

    CHECK(!game->visible());
    CHECK(!game->mouseFocusing());
    CHECK(!game->kbdFocusing());

    evt.active.gain = true;

    game->handleEvent(evt);

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

    game->handleEvent(evt);

    CHECK(!game->keyPressed(ANY_KEY_PRESSED));
    evt.type = SDL_KEYDOWN;

    MockKeyboard[SDLK_RETURN] = true;
    mock().expectOneCall("SDL_GetKeyState").andReturnValue(MockKeyboard);

    game->handleEvent(evt);

    CHECK(game->keyPressed(SDLK_RETURN));
    CHECK(game->keyPressed(ANY_KEY_PRESSED));
    evt.type = SDL_MOUSEMOTION;
    evt.motion.x = 15;
    evt.motion.y = 20;

    game->handleEvent(evt);

    CHECK_EQUAL(15, game->getMouse()->X);
    CHECK_EQUAL(20, game->getMouse()->Y);

    evt.type = SDL_MOUSEBUTTONDOWN;

    game->handleEvent(evt);

    CHECK(game->getMouse()->leftclick);

    evt.type = SDL_MOUSEBUTTONUP;

    game->handleEvent(evt);

    CHECK(!game->getMouse()->leftclick);
}

//int main(int ac, char** av)
//{
//    return CommandLineTestRunner::RunAllTests(ac, av);
//}
