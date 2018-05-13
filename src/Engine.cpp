#include <string>
#include <ios>
//#include <experimental/filesystem>

#include "GLoid.h"
#include "Point.h"

#include "Engine.h"
#include "World.h"
#include "Game.h"

//APP_NAME + " V" + APP_VERSION +
template<> Engine<Game>::Engine(){
//init log
    logStream.open(LOG_FILE, std::ios::out | std::ios::trunc);
    log(&logStream,
        std::string(APP_NAME),
        version(APP_VERSION).toString(),
        " -- Log Init...");

// Initialize the SDL library
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0){
        log(&logStream,
            "Unable to open SDL: ",
            SDL_GetError());
        exit(1);
    }else{
        log(&logStream,
            "SDL ",
            version(SDL_MAJOR_VERSION,
                    SDL_MINOR_VERSION,
                    SDL_PATCHLEVEL).toString(),
            ": Initialized! ");
    }
    // Initialize timer
    tic = SDL_GetTicks();
    // Initialize rand()
    srand(tic);

////Init font
//    if(TTF_Init() == -1)
//       {/*
//          log(TTF_GetError());
//          log(": Unable to initialize SDL_ttf\n");
//          return FALSE;*/
//       }
//       dejaVuSans = TTF_OpenFont((std::string(WORKPATH)
//                                  + "/DejaVuSans.ttf").c_str(),
//                                 fontSize);

//       if(dejaVuSans == NULL){
//   //       Log(TTF_GetError());
//   //       Log(": Unable to Load Font\n");
//   //       return FALSE;
//       }else{
//           TTF_SetFontStyle(dejaVuSans, TTF_STYLE_BOLD);
//        }
////init audio
//    // 16-bit stereo audio at 22kHz
//    SDL_AudioSpec sdlAudio;
//    sdlAudio.freq = 22050;
//    sdlAudio.format = AUDIO_S16;
//    sdlAudio.channels = 2;
//    sdlAudio.samples = 512;
//    sdlAudio.callback = NULL;//FIXME: mixer;
//    sdlAudio.userdata = NULL;

//    // Open the audio device
//    if(SDL_OpenAudio(&sdlAudio, NULL) < 0){
//        std::cout   << "Unable to open audio:"
//                    << SDL_GetError()
//                    << std::endl;
//        exit(1);
//    }
//    // Start playing whatever is in the buffer
//    SDL_PauseAudio(0);

////initialize OpenGL

//    // Black background
//    glClearColor(ZERO, ZERO, ZERO, ZERO);

//    // Set up depth buffer
//    glClearDepth(ONE);

//    // Set the type of depth testing
//    glDepthFunc(GL_LEQUAL);

//    //       if(Option_smooth)
//    //        glShadeModel(GL_SMOOTH);
//    //   else
//    glShadeModel(GL_FLAT);

//    // Set perspective calculations to most accurate
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//    // Set point smoothing to nicest
//    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

//    // Set the blending function
//    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

//    // Enable multisampling
//#ifndef WIN32
//    if(multisampleBuf > 0)
//        glEnable(GL_MULTISAMPLE_ARB);
//#endif
//    // Enable alpha blending
//    glEnable(GL_BLEND);

//    // Enable texture mapping
//    glEnable(GL_TEXTURE_2D);

//    // Set up the lighting parameters
//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

//    // Set up the OpenGL view
//    glViewport(0, 0, (GLsizei)(scr.W),(GLsizei)(scr.H));

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(90.0f, (GLfloat)(scr.W)/(GLfloat)(scr.H), 0.1f, 500.0f);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(ZERO, ZERO, 10.0f ,
//              ZERO, ZERO, -SCENE_MAX,
//              ZERO, ONE, ZERO);

//    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

template<> Engine<Game>::~Engine(){
    logStream.flush();
    logStream.close();
}

template<> screen_t Engine<Game>::getScreen()  {
    return &scr;
}

template<> mousecntl_t Engine<Game>::getMouse()  {
    return &mouse;
}

template<> void Engine<Game>::printText(bool option,
                     text2d* text,
                     SDL_Color fg,
                     SDL_Color bg,
                     int x,
                     int y,
                     const char* buf, ...){
    // extern TTF_Font *DejaVuSans;
     va_list Arg;

     va_start(Arg, buf);
     vsprintf(text->msg,buf, Arg);
     va_end(Arg);
  /*
     if(option)
        text->T = TTF_RenderText_Shaded(DejaVuSans, text->msg, fg, bg);
     else
        text->T = TTF_RenderText_Blended(DejaVuSans, text->msg, fg);
  */
     text->src.w = text->T->w;
     text->src.h = text->T->h;
     text->src.x = x;
     text->src.y = y;
}

template<> Game* Engine<Game>::withSdlGlVideo(version &v){
    if(v.value() < 0x010209){
        log(&logStream,
            "libSDL ",
            v.toString(),
            " doesn't support SDL_GetVideoInfo(), testing video modes...");
        pairs::const_iterator it = videoModes.begin();
        while(it++ != videoModes.end() && sdlScreen == NULL){
            for(unsigned char bpp = 32; bpp > 8;  bpp -= 8){
                sdlScreen = SDL_SetVideoMode(it->first, it->second, bpp, vFlags);
                if(sdlScreen != NULL){
                      scr = screen(it->first, it->second, bpp);
                      break;
                 }
            }
        }
    }else{
        desktop = SDL_GetVideoInfo();
        for(unsigned char bpp = 32; bpp > 8; bpp -= 8){
            sdlScreen = SDL_SetVideoMode(desktop->current_w,
                                         desktop->current_h,
                                         bpp, vFlags);
            if(sdlScreen != NULL){
                  scr = screen(desktop->current_w,
                               desktop->current_h, bpp);
                  break;
             }
        }
    }
    if(sdlScreen == NULL){
              log(&logStream,
                  "Can't open an SDL Screen:",
                  SDL_GetError());
    }else{
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, scr.BPP);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, multisampleBuf);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisampleSamples);
        // Hide system cursor
        SDL_ShowCursor(0);
        fontSize = scr.H/16;
    }
    return static_cast<Game*>(this);
}

template<> int Engine<Game>::now(){
    return SDL_GetTicks();
}
