#include "GLoid.h"
#include <sstream>
//#include <functional>
//#include <optional>
#include <vector>
#include <map>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "Point.h"
#include "Engine.h"
#include "Game.h"

//#include "World.h"

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

void SDL_WM_SetCaption(const char *title, const char *icon){
    mock().actualCall("SDL_WM_SetCaption");
}

int SDL_PollEvent(SDL_Event *event){
    return mock().actualCall("SDL_PollEvent")
                .returnIntValue();
}

int SDL_WaitEvent(SDL_Event *event){
    return mock().actualCall("SDL_WaitEvent")
                .returnIntValue();
}
unsigned char MockKeyboard[SDLK_LAST] = {0};

unsigned char * SDL_GetKeyState(int *numkeys){
    mock().actualCall("SDL_GetKeyState");
    //            .returnPointerValue();
    return MockKeyboard;
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
    mock().actualCall("TTF_OpenFont");
    return (TTF_Font *)NULL;
        /*mock().actualCall("TTF_OpenFont")
                .returnPointerValue();*/
}

void TTF_SetFontStyle(TTF_Font *font, int style){
    mock().actualCall("TTF_SetFontStyle");
}

SDL_Surface * TTF_RenderText_Shaded
(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg){
    mock().actualCall("TTF_RenderText_Shaded");
    return (SDL_Surface * )NULL;
}

SDL_Surface * TTF_RenderText_Blended
(TTF_Font *font,const char *text, SDL_Color fg){
    mock().actualCall("TTF_RenderText_Blended");
    return (SDL_Surface * )NULL;
}

//audio

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained){
    return mock().actualCall("SDL_OpenAudio")
                .returnIntValue();
}

void SDL_PauseAudio(int pause_on){
    mock().actualCall("SDL_PauseAudio");
}

void  SDL_MixAudio(Uint8 *dst, const Uint8 *src, Uint32 len, int volume){
    mock().actualCall("SDL_MixAudio");
}

int SDL_BuildAudioCVT(SDL_AudioCVT *cvt,
                      Uint16 src_format, Uint8 src_channels, int src_rate,
                      Uint16 dst_format, Uint8 dst_channels, int dst_rate){
    mock().actualCall("SDL_BuildAudioCVT");

    return 0;
}

int SDL_ConvertAudio(SDL_AudioCVT *cvt){
    mock().actualCall("SDL_ConvertAudio");

    return 0;
}

struct SDL_Surface Surf;

SDL_Surface * SDL_CreateRGBSurface
(Uint32 flags, int width, int height, int depth,
 Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask){
    return (SDL_Surface *)mock().actualCall("SDL_CreateRGBSurface")
            .returnPointerValueOrDefault(&Surf);
}

int SDL_SetColorKey
(SDL_Surface *surface, Uint32 flag, Uint32 key){
    mock().actualCall("SDL_SetColorKey");
}

int SDL_FillRect
(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color){
    mock().actualCall("SDL_FillRect");
}

int SDL_UpperBlit
(SDL_Surface *src, SDL_Rect *srcrect,
 SDL_Surface *dst, SDL_Rect *dstrect){
    mock().actualCall("SDL_UpperBlit");
}

void SDL_FreeSurface(SDL_Surface *surface){
    mock().actualCall("SDL_FreeSurface");
}
Uint32 SDL_MapRGBA
(const SDL_PixelFormat * const format,
 const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a){
    mock().actualCall("SDL_MapRGBA");
}

//openGL
void glColor3f( float x, float y, float z){
    mock().actualCall("glColor3f");
}

void glClearColor
( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha ){
    mock().actualCall("glClearColor");
}


void glTexParameteri( GLenum target, GLenum pname, GLint param ){
    mock().actualCall("glTexParameteri");
}

void glTexImage2D( GLenum target, GLint level,
                   GLint internalFormat,
                   GLsizei width, GLsizei height,
                   GLint border, GLenum format, GLenum type,
                   const GLvoid *pixels ){
    mock().actualCall("glTexImage2D");
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

void glBegin(unsigned int mode){
    mock().actualCall("glBegin");
}

void glEnd(){
    mock().actualCall("glEnd");
}

void glVertex3f(float x, float y, float z){
    mock().actualCall("glVertex3f");
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

void glTexCoord2f( GLfloat s, GLfloat t ){
    mock().actualCall("glTexCoord2f");
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

Game * newGame(){
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

    delete game;
    delete text;
}

TEST(GameTestGroup, time_test){
    Game * game = newGame();
    mock().expectOneCall("SDL_GetTicks");
    game->toc();
    mock().checkExpectations();
    delete game;
}

TEST(GameTestGroup, loop_test){
    Game * game = newGame();

    mock().expectOneCall("SDL_PollEvent").andReturnValue(1);
    game = game->loop();

    SDL_Event evt;
    evt.type = SDL_QUIT;

    game = game->handleEvent(evt);

    CHECK(!game->looping());

    evt.type = SDL_VIDEORESIZE;
    evt.resize.w = 800;
    evt.resize.h = 600;
    mock().expectOneCall("glViewport");
    mock().expectNCalls(2,"glLoadIdentity");
    mock().expectNCalls(2,"glMatrixMode");
    mock().expectOneCall("gluPerspective");
    mock().expectOneCall("gluLookAt");

    game = game->handleEvent(evt);

    evt.type = SDL_ACTIVEEVENT;
    evt.active.state = SDL_APPACTIVE | SDL_APPMOUSEFOCUS | SDL_APPINPUTFOCUS;
    evt.active.gain = false;

    game = game->handleEvent(evt);

    CHECK(!game->visible());
    CHECK(!game->mouseFocusing());
    CHECK(!game->kbdFocusing());
    //app is not visible, next loop should block on SDL_WaitEvent
    mock().expectOneCall("SDL_PollEvent").andReturnValue(0);
    mock().expectOneCall("SDL_WaitEvent");
    game = game->loop();

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

    CHECK(game->getFont() == NULL);
    mock().checkExpectations();
/*ttf_init succeeds, ttf_openfont fails*/
    mock().expectOneCall("TTF_Init").andReturnValue(0);
    mock().expectOneCall("TTF_OpenFont");
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game = game->withSdlTtf("./DejaVuSans.ttf");
    CHECK(game->getFont() == NULL);
    mock().checkExpectations();
/*sdl audio*/
/*openaudio fails*/
    mock().expectOneCall("SDL_OpenAudio").andReturnValue(-1);
    mock().expectOneCall("SDL_GetError").andReturnValue("error");

    game = game->withSdlAudio(22050, 2, 0);

    mock().checkExpectations();
    CHECK(game->getSdlAudio() == NULL);

/*openaudio succeds*/
    mock().expectOneCall("SDL_OpenAudio").andReturnValue(0);
    mock().expectOneCall("SDL_PauseAudio");

    game = game->withSdlAudio(22050, 2, 512);

    mock().checkExpectations();
    CHECK(game->getSdlAudio() != NULL);

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

//TEST(GameTestGroup, step_factory_test){
//    Game * game = newGame();
//    Step s = game->next(NULL);
//    CHECK_EQUAL(STEP_LOADING, s.type);
//    s = Step(STEP_LOADING);
//    s = game->next(&s);
//    delete game;
//}

TEST(GameTestGroup, sound_test){
    Game::mixer(NULL, NULL, 0);
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
    game = game->addSound(loaded, 8, 0);

    bool succ = game->playSound(-1);
    CHECK_EQUAL(0, game->pendingSounds());
    CHECK(!succ);

    succ = game->playSound(0);
    CHECK(succ);
    CHECK_EQUAL(1, game->pendingSounds());

    delete game;
}



int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
