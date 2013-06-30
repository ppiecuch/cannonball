#pragma once

#include "stdint.hpp"

// ------------------------------------------------------------------------------------------------
// Compiler Settings
// ------------------------------------------------------------------------------------------------

// Comment out to disable SDL specific sound code
#define COMPILE_SOUND_CODE 1

// ------------------------------------------------------------------------------------------------
// Settings
// Some of these will move to the options class, once they are dynamic and controllable via a menu
// ------------------------------------------------------------------------------------------------

const bool DEBUG_LEVEL = false;

// ------------------------------------------------------------------------------------------------
// General useful stuff
// ------------------------------------------------------------------------------------------------

// Internal Sega OutRun Screen Properties
const uint16_t S16_WIDTH      = 320;
const uint16_t S16_HEIGHT     = 224;

// Internal Widescreen Width
const uint16_t S16_WIDTH_WIDE = 398;

const uint16_t S16_PALETTE_ENTRIES = 0x1000;

enum
{
    BIT_0 = 0x01,
    BIT_1 = 0x02,
    BIT_2 = 0x04,
    BIT_3 = 0x08,
    BIT_4 = 0x10,
    BIT_5 = 0x20,
    BIT_6 = 0x40,
    BIT_7 = 0x80,
    BIT_8 = 0x100,
    BIT_9 = 0x200,
    BIT_A = 0x400
};