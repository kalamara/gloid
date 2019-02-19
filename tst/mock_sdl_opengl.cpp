#include "GLoid.h"
#include "mock_sdl_opengl.h"
#include "CppUTestExt/MockSupport.h"


//SDL
struct SDL_Surface MockSurf;
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

int SDL_PushEvent(SDL_Event *event){
    return mock().actualCall("SDL_PushEvent")
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
nullptr,/**< Value: The format of the video surface */
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
    return (SDL_Surface * )nullptr;/*(SDL_Surface * )mock().actualCall("SDL_SetVideoMode")
                .returnPointerValue();*/
}

void SDL_GL_SwapBuffers(void){
    mock().actualCall("SDL_GL_SwapBuffers");
}

int SDL_GL_SetAttribute(SDL_GLattr attr, int value){
    return mock().actualCall("SDL_GL_SetAttribute")
                .returnIntValue();
}

int SDL_ShowCursor(int toggle){
    return mock().actualCall("SDL_ShowCursor")
                .returnIntValue();
}

SDL_RWops * SDL_RWFromFile(const char *file, const char *mode){
    mock().actualCall("SDL_RWFromFile");
    return (SDL_RWops *)nullptr;
}

SDL_AudioSpec * SDL_LoadWAV_RW(SDL_RWops *src,
                               int freesrc,
                               SDL_AudioSpec *spec,
                               Uint8 **audio_buf,
                               Uint32 *audio_len){
    mock().actualCall("SDL_LoadWAV_RW");
    return (SDL_AudioSpec *)nullptr;
}

//TTF

int TTF_Init(void){
    return mock().actualCall("TTF_Init")
                .returnIntValue();
}
TTF_Font * MockFont = (TTF_Font *)nullptr;

TTF_Font * TTF_OpenFont(const char *file, int ptsize){
    mock().actualCall("TTF_OpenFont");
    return (TTF_Font *)nullptr;
        /*mock().actualCall("TTF_OpenFont")
                .returnPointerValue();*/
}

void TTF_SetFontStyle(TTF_Font *font, int style){
    mock().actualCall("TTF_SetFontStyle");
}

SDL_Surface * TTF_RenderText_Shaded
(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg){
    mock().actualCall("TTF_RenderText_Shaded");
    return &MockSurf;
}

SDL_Surface * TTF_RenderText_Blended
(TTF_Font *font,const char *text, SDL_Color fg){
    mock().actualCall("TTF_RenderText_Blended");
    return &MockSurf;
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



SDL_Surface * SDL_CreateRGBSurface
(Uint32 flags, int width, int height, int depth,
 Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask){
    return (SDL_Surface *)mock().actualCall("SDL_CreateRGBSurface")
            .returnPointerValueOrDefault(&MockSurf);
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

SDL_Surface * SDL_LoadBMP_RW(SDL_RWops *src, int freesrc){
    mock().actualCall("SDL_LoadBMP_RW");
}

//OpenGL
void glPushMatrix(){
    mock().actualCall("glPushMatrix");
}

void glPopMatrix(){
    mock().actualCall("glPopMatrix");
}

void glBegin(unsigned int mode){
    mock().actualCall("glBegin");
}

void glEnd(){
    mock().actualCall("glEnd");
}

void glEnable( GLenum cap ){
    mock().actualCall("glEnable");
}

void glDisable( GLenum cap ){
    mock().actualCall("glDisable");
}

void glScalef( GLfloat x, float y, float z ){
    mock().actualCall("glScalef");
}

void glTranslatef(float x, float y, float z){
    mock().actualCall("glTranslatef");
}

void glRotatef(float a, float x, float y, float z){
    mock().actualCall("glRotatef");
}

void glVertex3f(float x, float y, float z){
    mock().actualCall("glVertex3f");
}

void glColor4f( float x, float y, float z, float a){
    mock().actualCall("glColor4f");
}

void glColor3f( float x, float y, float z){
    mock().actualCall("glColor3f");
}

void glClearColor
( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha ){
    mock().actualCall("glClearColor");
}

void glNormal3f( float x, float y, float z){
    mock().actualCall("glNormal3f");
}

void glTexEnvf( GLenum target, GLenum pname, GLfloat param ){
    mock().actualCall("glTexEnvf");
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

void glGenTextures( GLsizei n, GLuint *textures ){
    if(textures &&
    n > 0){
        textures[n-1] = n;
    }
    mock().actualCall("glGenTextures");
}

void glBindTexture( GLenum target, GLuint texture ){
    mock().actualCall("glBindTexture");
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
GLUquadric * gluNewQuadric(){
    mock().actualCall("gluNewQuadric");
}

void gluDeleteQuadric(GLUquadric * base){
    mock().actualCall("gluDeleteQuadric");
}

void gluQuadricDrawStyle (GLUquadric* quad, GLenum draw){
    mock().actualCall("gluQuadricDrawStyle");
}

void gluQuadricTexture (GLUquadric* quad, GLboolean texture){
    mock().actualCall("gluQuadricTexture");
}

void gluSphere( GLUquadric * b, double r, int sl, int st){
    mock().actualCall("gluSphere");
}

void gluCylinder (GLUquadric* quad, double base, double top, double height, int slices, int stacks){
    mock().actualCall("gluCylinder");
}

void gluQuadricOrientation(GLUquadric* quad, GLenum orientation){
    mock().actualCall("gluQuadricOrientation");
}

void gluDisk(GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops){
    mock().actualCall("gluDisk");
}

