# Default CMake Setup. Used for Win32 Builds.
# For DirectX, assumes DXSDK is already configured and pointing to your DirectX setup.

set(lib_base c:/coding/lib)
set(sdl_root ${lib_base}/SDL-1.2.15)

if (NOT DEFINED ENV{DXSDK})
    message(FATAL_ERROR "Warning: DirectX SDK Variable DXSDK Not Defined!")
endif()

include_directories(
    "${sdl_root}/include"
)

link_libraries(cannonball 
    SDL
    SDLmain
    dxguid
    dinput8
)

# Linking
link_directories(
    "$ENV{DXSDK}/lib"
    "${sdl_root}/lib"
)
 
# Location for Cannonball to create save files
# Used to auto-generate setup.hpp with various file paths
set(xml_directory ./)
set(sdl_flags "SDL_DOUBLEBUF | SDL_SWSURFACE")