#include "GLoid.h"
#include <sstream>
//#include <functional>
//#include <optional>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "Point.h"
#include "Engine.h"
#include "World.h"
#include "Game.h"

//C++ libraries

void srand(unsigned int s) {
    mock().actualCall("srand");
}

//SDL

int SDL_Init(Uint32 flags){
    return mock().actualCall("SDL_Init")
                .returnIntValue();
}
char e[6] = "error";
char * SDL_GetError(void){
    mock().actualCall("SDL_GetError");
    return e;
    /*(char *)mock().actualCall("SDL_GetError")
                .returnPointerValue();*/
}

Uint32 SDL_GetTicks(void){
    return mock().actualCall("SDL_GetTicks")
                .returnIntValue();
}
SDL_VideoInfo MockDesktop = {
1,/**< Flag: Can you create hardware surfaces? */
1,	/**< Flag: Can you talk to a window manager? */
0,
0,
1,/**< Flag: Accelerated blits HW --> HW */
1,/**< Flag: Accelerated blits with Colorkey */
1, /**< Flag: Accelerated blits with Alpha */
1,/**< Flag: Accelerated blits SW --> HW */
1,/**< Flag: Accelerated blits with Colorkey */
1,/**< Flag: Accelerated blits with Alpha */
1,/**< Flag: Accelerated color fill */
0,
2,/**< The total amount of video memory (in K) */
NULL,/**< Value: The format of the video surface */
800,/**< Value: The current video mode width */
600/**< Value: The current video mode height */
};
const SDL_VideoInfo * SDL_GetVideoInfo(void){
    /*return (const SDL_VideoInfo * )mock().actualCall("SDL_GetVideoInfo")
                .returnPointerValue();*/
    mock().actualCall("SDL_GetVideoInfo");
    return &MockDesktop;
}

SDL_Surface * SDL_SetVideoMode
            (int width, int height, int bpp, Uint32 flags){
    mock().actualCall("SDL_SetVideoMode");
    return (SDL_Surface * )NULL;/*(SDL_Surface * )mock().actualCall("SDL_SetVideoMode")
                .returnPointerValue();*/
}

int SDL_GL_SetAttribute(SDL_GLattr attr, int value){
    return mock().actualCall("SDL_GL_SetAttribute")
                .returnIntValue();
}

int SDL_ShowCursor(int toggle){
    return mock().actualCall("SDL_ShowCursor")
                .returnIntValue();
}

//TTF

int TTF_Init(void){
    return mock().actualCall("TTF_Init")
                .returnIntValue();
}

TTF_Font * TTF_OpenFont(const char *file, int ptsize){
    return (TTF_Font *)mock().actualCall("TTF_OpenFont")
                .returnPointerValue();
}

void TTF_SetFontStyle(TTF_Font *font, int style){
    mock().actualCall("TTF_SetFontStyle");
}

//audio

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained){
    return mock().actualCall("SDL_OpenAudio")
                .returnIntValue();
}

void SDL_PauseAudio(int pause_on){
    mock().actualCall("SDL_PauseAudio");
}

//openGL
void glClearColor
( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha ){
    mock().actualCall("voidglClearColor");
}

void glClearDepth( GLclampd depth ){
    mock().actualCall("glClearDepth");
}

void glDepthFunc( GLenum func ){
    mock().actualCall("glDepthFunc");
}

void glShadeModel( GLenum mode ){
    mock().actualCall("glShadeModel");
}

void glEnable( GLenum cap ){
    mock().actualCall("glEnable");
}

void glDisable( GLenum cap ){
    mock().actualCall("glDisable");
}

void glHint( GLenum target, GLenum mode ){
    mock().actualCall("glHint");
}

void glBlendFunc( GLenum sfactor, GLenum dfactor ){
    mock().actualCall("glBlendFunc");
}

void glLightfv( GLenum light, GLenum pname,
                const GLfloat *params ){
    mock().actualCall("glLightfv");
}

void glMaterialfv( GLenum face, GLenum pname, const GLfloat *params ){
    mock().actualCall("glMaterialfv");
}

void glViewport( GLint x, GLint y,
                GLsizei width, GLsizei height ){
    mock().actualCall("glViewport");
}

void glMatrixMode( GLenum mode ){
    mock().actualCall("glMatrixMode");
}

void glLoadIdentity( void ){
    mock().actualCall("glLoadIdentity");
}

void glClear( GLbitfield mask ){
    mock().actualCall("glClear");
}

//GLU extentions

void gluPerspective
(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar){
    mock().actualCall("gluPerspective");
}

void gluLookAt
(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
 GLdouble centerX, GLdouble centerY, GLdouble centerZ,
 GLdouble upX, GLdouble upY, GLdouble upZ){
    mock().actualCall("gluLookAt");
}

TEST_GROUP(GameTestGroup){
    void teardown(){
            mock().clear();
    }
};

TEST(GameTestGroup, log_test){
    std::stringstream str;
    Engine<Game>::log(&str, "ena =");

    STRCMP_EQUAL("ena =\n", str.str().c_str());

    str.str( std::string() );
    str.clear();

    Engine<Game>::log(&str, "ena = ", 1, ",~dyo ison ", 2.00001f);

    STRCMP_EQUAL("ena = 1,~dyo ison 2.00001\n", str.str().c_str());

    //std::cout << str.str();
}

TEST(GameTestGroup, time_test){
    mock().expectOneCall("SDL_Init");
    mock().expectOneCall("SDL_GetTicks")
            .andReturnValue(123);
    mock().expectOneCall("srand");
    Game * game = new Game();
    mock().expectOneCall("SDL_GetTicks");
    game->now();
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

TEST(GameTestGroup, ctor_dtor_test){
    mock().expectOneCall("SDL_Init");
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
/*
    //newer sdl library -> call sdlgetvideoinfo
    sdlv = version(1,2,9);

    mock().expectOneCall("SDL_GetVideoInfo");
    mock().expectNCalls(3, "SDL_SetVideoMode");
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game = game->withSdlGlVideo(sdlv);

    mock().checkExpectations();*/
    delete game;
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
