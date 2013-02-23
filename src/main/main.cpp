/***************************************************************************
    Cannonball Main Entry Point.
    
    Copyright Chris White.
    See license.txt for more details.
***************************************************************************/

// Error reporting
#include <iostream>

// SDL Library
#include <SDL.h>
#pragma comment(lib, "SDLmain.lib") // Replace main with SDL_main
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "glu32.lib")

// SDL Specific Code
#include "sdl/timer.hpp"
#include "sdl/input.hpp"
#include "sdl/video.hpp"

#include "romloader.hpp"
#include "stdint.hpp"
#include "main.hpp"
#include "setup.hpp"
#include "engine/outrun.hpp"
#include "frontend/config.hpp"
#include "frontend/menu.hpp"
#include "tracked/tracked.hpp"

// Direct X Haptic Support.
// Fine to include on non-windows builds as dummy functions used.
#include "directx/ffeedback.hpp"

// Initialize Shared Variables
using namespace cannonball;

int cannonball::state       = STATE_BOOT;
int cannonball::frame_ms    = 0;
int cannonball::frame       = 0;
bool cannonball::tick_frame = true;

#ifdef COMPILE_SOUND_CODE
Audio cannonball::audio;
#endif

Menu menu;
Tracked tracked;

static void quit_func(int code)
{
#ifdef COMPILE_SOUND_CODE
    audio.stop_audio();
#endif
    input.close();
    forcefeedback::close();
    SDL_Quit();
    exit(code);
}

static void process_events(void)
{
    SDL_Event event;

    // Grab all events from the queue.
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                // Handle key presses.
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    state = STATE_QUIT;
                else
                    input.handle_key_down(&event.key.keysym);
                break;

            case SDL_KEYUP:
                input.handle_key_up(&event.key.keysym);
                break;

            case SDL_JOYAXISMOTION:
                input.handle_joy_axis(&event.jaxis);
                break;

            case SDL_JOYBUTTONDOWN:
                input.handle_joy_down(&event.jbutton);
                break;

            case SDL_JOYBUTTONUP:
                input.handle_joy_up(&event.jbutton);
                break;

            case SDL_QUIT:
                // Handle quit requests (like Ctrl-c).
                state = STATE_QUIT;
                break;
        }
    }
}

// Pause Engine
bool pause_engine;

static void tick()
{
    frame++;

    // Non standard FPS.
    // Determine whether to tick the current frame.
    if (config.fps != 30)
    {
        if (config.fps == 60)
            tick_frame = frame & 1;
        else if (config.fps == 120)
            tick_frame = (frame & 3) == 1;
    }

    process_events();

    switch(state)
    {
        case STATE_GAME:
        {
            if (input.has_pressed(Input::TIMER))
                outrun.freeze_timer = !outrun.freeze_timer;

            if (input.has_pressed(Input::PAUSE))
                pause_engine = !pause_engine;

            if (input.has_pressed(Input::MENU))
                state = STATE_INIT_MENU;

            if (!pause_engine || input.has_pressed(Input::STEP))
            {
                outrun.tick(tick_frame);
                input.frame_done(); // Denote keys read

                #ifdef COMPILE_SOUND_CODE
                // Tick audio program code
                osoundint.tick();
                // Tick SDL Audio
                audio.tick();
                #endif
            }
            else
            {                
                input.frame_done(); // Denote keys read
            }
        }
        break;

        case STATE_INIT_GAME:
            if (config.engine.jap && !roms.load_japanese_roms())
            {
                state = STATE_QUIT;
            }
            else
            {
                outrun.init();
                state = STATE_GAME;
            }
            break;

        case STATE_MENU:
        {
            menu.tick();
            input.frame_done();
            #ifdef COMPILE_SOUND_CODE
            // Tick audio program code
            osoundint.tick();
            // Tick SDL Audio
            audio.tick();
            #endif
        }
        break;

        case STATE_INIT_MENU:
            menu.init();
            state = STATE_MENU;
            break;

        case STATE_TRACKED:
            tracked.tick();
            break;
    }
    // Draw SDL Video
    video.draw_frame();  
}

static void main_loop()
{
    Timer fps;

    int t;
    int deltatime = 0;

    while (state != STATE_QUIT)
    {
        // Start the frame timer
        fps.start();
        tick();
        #ifdef COMPILE_SOUND_CODE
        deltatime = (int) (frame_ms * audio.adjust_speed());
        #else
        deltatime = frame_ms;
        #endif
        t = fps.get_ticks();

        // Cap Frame Rate
        if (t < deltatime)
        {
            //Sleep the remaining frame time
            SDL_Delay( deltatime - t );
        }
    }

    quit_func(0);
}

int main(int argc, char* argv[])
{
    // Initialize timer and video systems
    if( SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == -1 ) 
    { 
        std::cerr << "SDL Initialization Failed: " << SDL_GetError() << std::endl;
        return 1; 
    } 

    // Load Roms
    bool roms_loaded = roms.load_revb_roms();

    if (roms_loaded)
    {
        // Load XML Config
        config.load(FILENAME_CONFIG);

        //Set the window caption 
        SDL_WM_SetCaption( "Cannonball", NULL ); 

        // Initialize SDL Video
        if (!video.init(&roms, &config.video))
            quit_func(1);

#ifdef COMPILE_SOUND_CODE
        audio.init();
#endif
        //state = config.menu.enabled ? STATE_INIT_MENU : STATE_INIT_GAME;
        state = STATE_TRACKED;

        // Initalize controls
        input.init(config.controls.keyconfig, config.controls.padconfig, 
                   config.controls.analog,    config.controls.axis, config.controls.wheel);

        if (config.controls.haptic) 
            config.controls.haptic = forcefeedback::init(config.controls.max_force, config.controls.min_force, config.controls.force_duration);
        
        // Populate menus
        menu.populate();
        main_loop();  // Loop until we quit the app
    }
    else
    {
        quit_func(1);
    }

    // Never Reached
    return 0;
}
