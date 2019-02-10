#include <string>
#include <ios>
//#include <experimental/filesystem>
#include <algorithm>
#include "GLoid.h"

//APP_NAME + " V" + APP_VERSION +
template<> Engine<Game>::Engine(){
//init log
    logStream.open(LOG_FILE, std::ios::out | std::ios::trunc);
        info(std::string(APP_NAME),
        version(APP_VERSION).toString(),
        " -- Log Init...");

// Initialize the SDL library
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0){
        error("Unable to open SDL: ",
              SDL_GetError());
        exit(1);
    }else{
       info("SDL ",
            version(SDL_MAJOR_VERSION,
                    SDL_MINOR_VERSION,
                    SDL_PATCHLEVEL).toString(),
            ": Initialized! ");
       // Set the window caption
       SDL_WM_SetCaption(APP_NAME, nullptr);
    }
    // Initialize timer
    tic = SDL_GetTicks();
    // Initialize rand()
    srand(tic);
}

template<> Engine<Game>::~Engine(){
    logStream.close();
}

template<> screenopt Engine<Game>::getScreen() {
    return sdlScreen;
}

template<> mousecntl_t Engine<Game>::getMouse()  {
    return &mouse;
}

template<> screenopt Engine<Game>::testVmode(unsigned x, unsigned int y){

    for(unsigned char bpp = 32; bpp > 8;  bpp -= 8){
        auto s = SDL_SetVideoMode(x, y, bpp, vFlags);
        if(s){

             return screen(x, y, bpp, s);
         }
    }
    return {};
}

template<> void Engine<Game>::mixer(void *udata, Uint8 *stream, int len){
    auto *buf = (std::vector<struct sbuffer> *)udata;
    if(buf){
        std::transform(begin(*buf),
                       end(*buf),
                       begin(*buf),
                       [stream, len](struct sbuffer s) -> struct sbuffer{
            if(s.data){
                // How much is left to play?
                int amount = (s.dlen > len) ? len : s.dlen;
                auto traf = (amount < s.dlen) ?
                                            &(s.data[amount]) :
                                            (unsigned char *)"";
                SDL_MixAudio(stream,
                             s.data,
                             amount, SDL_MIX_MAXVOLUME);

                return sbuffer(traf, s.dlen - amount);
            }
            return s; //sapio
        });
    }
}

template<> Game* Engine<Game>::addSound(unsigned char * data,
                                        unsigned int dlen,
                                        const std::string & key){
    if(sdlAudio){
        SDL_AudioCVT wave;
        SDL_BuildAudioCVT(&wave,
            sdlAudio->format,
            sdlAudio->channels,
            sdlAudio->freq,
            AUDIO_S16,
            sdlAudio->channels,
            sdlAudio->freq);

          wave.buf = (unsigned char *)malloc(dlen * wave.len_mult);

          memcpy(wave.buf, data, dlen);
          wave.len = dlen;

          SDL_ConvertAudio(&wave);
          sounds.emplace( key, wave);
    }
    return dynamic_cast<Game*>(this);
}

template<> Game* Engine<Game>::loadBmp(const std::string & name,
                                       GLuint id){
    auto path = std::string(WORKPATH) + "/textures/" + name + ".bmp" ;
    auto p = SDL_LoadBMP(path.c_str());
    if(p &&
       p->h &&
       p->w ){
        SDL_SetColorKey(p,
                    SDL_SRCCOLORKEY|SDL_RLEACCEL,
                    SDL_MapRGBA(p->format, 0, 0, 0, 0));
        textures.emplace(name, std::pair(id, *p));
        info("Loaded ", path);
        if(!ispoweroftwo(p->h)){
            warning("Height of ", name, " is not a power fof two");
        }
        if(!ispoweroftwo(p->w)){
            warning("Width of ", name, " is not a power fof two");
        }
    }else{
        error("Couldn't load ", path, ": ", SDL_GetError());
    }
    return dynamic_cast<Game*>(this);
}

template<> Game* Engine<Game>::loadSound(const std::string & name){
    Uint8 *data;
    Uint32 dlen;
    auto path = std::string(WORKPATH) + "/sounds/" + name + ".wav" ;
    if(getSdlAudio()){
        auto audio = getSdlAudio().value();
        if(SDL_LoadWAV(path.c_str(), &audio, &data, &dlen)){
            info("Loaded ", path);

            return addSound(data, dlen, name);
        }
    }
    error("Couldn't load ", path, ": ", SDL_GetError());

    return dynamic_cast<Game*>(this);
}

template<> bool Engine<Game>::playSound(const std::string & sound){

   std::remove_if(begin(soundBuffers),
                  end(soundBuffers),
                  [](auto s){
                return !s.data || !s.dlen;
            });

   auto found = sounds.find(sound);
   if(found != end(sounds)){
       soundBuffers.emplace_back(sbuffer(found->second.buf,
                                         found->second.len_cvt));

       return true;
   }
   return false;
}

template<> Game& Engine<Game>::withSdlGlVideo(version &v){
    if(v.value() < 0x010209){
        warning("libSDL ",
            v.toString(),
            " doesn't support SDL_GetVideoInfo(), testing video modes...");
        auto it = begin(videoModes);
        while(it++ != end(videoModes) && !sdlScreen.has_value()){
            sdlScreen = testVmode(it->first, it->second);
        }
    }else{
        desktop = SDL_GetVideoInfo();
        sdlScreen = testVmode(desktop->current_w,desktop->current_h);
    }
    if(!sdlScreen.has_value()){
             error("Can't open an SDL Screen:",
                  SDL_GetError());
    }else{
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, colorSize);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, sdlScreen->BPP);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, multisampleBuf);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisampleSamples);
        // Hide system cursor
        SDL_ShowCursor(0);
        fontSize = sdlScreen->H/16;
        info("SDL OpenGL desktop(",
             sdlScreen->W, "x", sdlScreen->H,
             ")initialized!");
    }
    return dynamic_cast<Game&>(*this);
}
/*(std::string(WORKPATH)+ "/DejaVuSans.ttf").c_str()*/
template<> Game& Engine<Game>::withSdlTtf(std::string fontPath){
    if(TTF_Init() < 0){
        error("Unable to initialize SDL_ttf: ",
              TTF_GetError());
    }else{
        auto font = TTF_OpenFont(fontPath.c_str(),
                                 fontSize);
        if(!font){
            error("Unable to initialize SDL_ttf: ",
                  TTF_GetError());
        }else{
            TTF_SetFontStyle(font, TTF_STYLE_BOLD);
            sdlFont = font;
            info("TTF font found!");
        }
    }
    return dynamic_cast<Game&>(*this);
}

template<> Game& Engine<Game>::withSdlAudio(int freq,
                                            unsigned char channels,
                                            unsigned int samples){
    auto audio = SDL_AudioSpec();

    audio.freq = freq;
    audio.format = AUDIO_S16;
    audio.channels = channels;
    audio.samples = samples;
    audio.callback = Engine<Game>::mixer;
    audio.userdata = (void *)&soundBuffers;

// Open the audio device
    if(SDL_OpenAudio(&audio, nullptr) < 0){
        error("Unable to open audio:",SDL_GetError());
    } else {// Start playing whatever is in the buffer
         info("Audio open!");
         SDL_PauseAudio(0);
         sdlAudio = audio;
    }
    return dynamic_cast<Game&>(*this);
}

template<> Game& Engine<Game>::withOpenGl(){
//initialize OpenGL

//    info("Black background");
    glClearColor(ZERO, ZERO, ZERO, ZERO);

//    info("Set up depth buffer");
    glClearDepth(ONE);

//    info("Set the type of depth testing");
    glDepthFunc(GL_LEQUAL);

    //       if(Option_smooth)
    //        glShadeModel(GL_SMOOTH);
    //   else
    glShadeModel(GL_FLAT);

//    info("Set perspective calculations to most accurate");
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//    info("Set point smoothing to nicest");
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

//    info("Set the blending function");
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

//    info("Enable multisampling");
#ifndef WIN32
    if(multisampleBuf > 0)
        glEnable(GL_MULTISAMPLE_ARB);
#endif
//    info("Enable alpha blending");
    glEnable(GL_BLEND);

//    info("Enable texture mapping");
    glEnable(GL_TEXTURE_2D);

//    info("Set up the lighting parameters");
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

    struct screen scr;
    if(!sdlScreen){
        warning("SDL desktop not initialized, falling back to screen defaults...");
        scr = screen();
    }else{
        scr = screen(sdlScreen->W,
                     sdlScreen->H,
                     sdlScreen->BPP,
                     sdlScreen->S);
    }
//    info("Set up the OpenGL view");
    glViewport(0, 0, (GLsizei)(scr.W),(GLsizei)(scr.H));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f,
                   (GLfloat)(scr.W)/(GLfloat)(scr.H),
                   0.1f,
                   500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ZERO, ZERO, 10.0f ,
              ZERO, ZERO, -SCENE_MAX,
              ZERO, ONE, ZERO);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    info("OpenGL initialized!");

    return dynamic_cast<Game&>(*this);
}

template<> unsigned int Engine<Game>::toc(){
        return SDL_GetTicks();
}

template<> void Engine<Game>::reshape(int width, int height)
{
   // Reset the current viewport
   glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Calculate the aspect ratio of the window
   gluPerspective(45.0f, (GLfloat)(width)/ (GLfloat)(height), 0.1f, 500.0f);

   camera.x = ZERO;
   camera.y = ZERO;
   camera.z = SCENE_AIR;  // Center of rotation = SCENE_AIR - SCENE_MAX

   phi = ZERO;
   theta = ZERO;

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Set the perspective
   gluLookAt(camera.x, camera.y, camera.z,
             ZERO, ZERO, -SCENE_MAX,
             ZERO, ONE, ZERO);
}

template<> SDL_Surface * Engine<Game>::print2d(text2d & text){
    if(sdlFont){
        auto font = sdlFont.value();

        if(text.blended){

            return TTF_RenderText_Shaded(font,
                                     text.trim().c_str(),
                                     text.foreground,
                                     text.background);
        }else{

            return TTF_RenderText_Blended(font,
                                      text.trim().c_str(),
                                      text.foreground);
        }
    }
    return nullptr;
}

template<> void Engine<Game>::draw2d(
        SDL_Surface *surf,
         int x,
         int y){
    if(surf && sdlScreen.has_value()){
        unsigned int w = nextpoweroftwo(surf->w);
        unsigned int h = nextpoweroftwo(surf->h);

        float xx = SCENE_MIN + 2 * SCENE_MAX * x / sdlScreen->W;
        float yy = -SCENE_MIN + 2 * SCENE_MAX * y / sdlScreen->H;

        SDL_Surface *s = SDL_CreateRGBSurface(surf->flags,
                                              w, h,
                                              sdlScreen->BPP,
                                              0x00ff0000, 0x0000ff00,
                                              0x000000ff, 0xff000000);

        SDL_SetColorKey(s, SDL_SRCCOLORKEY|SDL_RLEACCEL,
                        SDL_MapRGBA(s->format, 0, 0, 0, 0));

        SDL_BlitSurface(surf, 0, s, 0);

        glEnable(GL_TEXTURE_2D);
        //      glBindTexture(GL_TEXTURE_2D, TextureID[N_BMP]);//use warp ID + 1
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, s->pixels);

        glColor3f(ONE, ONE, ONE);

        glBegin(GL_QUADS);
        glTexCoord2f(ONE, ONE);
        glVertex3f(xx + fontSize * w / sdlScreen->W,
                   yy - fontSize * h / sdlScreen->H, -SCENE_AIR);
        glTexCoord2f(ONE, ZERO);
        glVertex3f(xx + fontSize * w / sdlScreen->W, yy, -SCENE_AIR);
        glTexCoord2f(ZERO, ZERO);
        glVertex3f(xx, yy, -SCENE_AIR);
        glTexCoord2f(ZERO, ONE);
        glVertex3f(xx, yy - fontSize * h / sdlScreen->H, -SCENE_AIR);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        SDL_FreeSurface(s);
    }
}

template<> bool Engine<Game>::keyPressed(unsigned char k){
        if(k==ANY_KEY_PRESSED){
            bool acc = false;
            for(auto i : keys){
                acc = acc || i.second ;
            }
            return acc;
        }
        return keys[k];
}

template<> Game& Engine<Game>::handleEvent(SDL_Event & e){
    switch(e.type){
        case SDL_QUIT:
            app.looping = false;
            //info("Frames skipped:", frameskips);
            info("Quitting!");
            break;

        case SDL_VIDEORESIZE:
            reshape(e.resize.w, e.resize.h);
            break;

        case SDL_ACTIVEEVENT:
                   // Visibility status has changed
            if(e.active.state & SDL_APPACTIVE){
                app.visible = e.active.gain;
            }
                   // Mouse focus status has changed
            if(e.active.state & SDL_APPMOUSEFOCUS){
                app.mouse_focus = e.active.gain;
            }
                   // Input focus status has changed
            if(e.active.state & SDL_APPINPUTFOCUS){
                app.keyboard_focus = e.active.gain;
            }
            break;

        case SDL_KEYDOWN:{
                   // Take a snapshot of the keyboard
            auto k = SDL_GetKeyState(nullptr);
            std::for_each(begin(keys),
                            end(keys),
                            [k](auto &i){
                i.second = k[i.first];
            });
        }   break;

        case SDL_MOUSEMOTION:
            mouse.X = e.motion.x;
            mouse.Y = e.motion.y;
            break;

        case SDL_MOUSEBUTTONDOWN:
             mouse.leftclick = true;
             break;

        case SDL_MOUSEBUTTONUP:
             mouse.leftclick = false;
             break;

        default: break;
    }
    return dynamic_cast<Game&>(*this);
}

// Terminate application
template<> void Engine<Game>:: terminate(void)
{
   static SDL_Event Q;

   Q.type = SDL_QUIT;

   // Push an SDL_QUIT event into the event queue
   if(SDL_PushEvent(&Q) == -1)
   {
      error("SDL_QUIT event can't be pushed: ", SDL_GetError());
      exit(1);
   }
}
