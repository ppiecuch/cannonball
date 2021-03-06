#pragma once

#include <SDL.h>
#include "stdint.hpp"
#include "roms.hpp"
#include "hwvideo/hwtiles.hpp"
#include "hwvideo/hwsprites.hpp"
#include "hwvideo/hwroad.hpp"

class hwsprites;

struct video_settings_t;

class Video
{
public:
	static const uint16_t S16_PALETTE_ENTRIES = 0x1000;

	hwsprites* sprite_layer;
    hwtiles* tile_layer;
	uint32_t *pixels;
    uint32_t *screen_pixels;

    bool enabled;

	Video();
    ~Video();
    
	int init(Roms* roms, video_settings_t* settings);
    int set_video_mode(video_settings_t* settings);
    void draw_frame();

    void clear_text_ram();
    void write_text8(uint32_t, const uint8_t);
	void write_text16(uint32_t*, const uint16_t);
	void write_text16(uint32_t, const uint16_t);
    void write_text32(uint32_t*, const uint32_t);
    void write_text32(uint32_t, const uint32_t);
    uint8_t read_text8(uint32_t);

    void clear_tile_ram();    
    void write_tile8(uint32_t, const uint8_t);
	void write_tile16(uint32_t*, const uint16_t);
	void write_tile16(uint32_t, const uint16_t);
    void write_tile32(uint32_t*, const uint32_t);
    void write_tile32(uint32_t, const uint32_t);
    uint8_t read_tile8(uint32_t);

	void write_sprite16(uint32_t*, const uint16_t);

	void write_pal8(uint32_t*, const uint8_t);
	void write_pal16(uint32_t*, const uint16_t);
	void write_pal32(uint32_t*, const uint32_t);
	void write_pal32(uint32_t, const uint32_t);
	uint8_t read_pal8(uint32_t);
	uint16_t read_pal16(uint32_t*);
	uint16_t read_pal16(uint32_t);
    uint32_t read_pal32(uint32_t*);

private:
	SDL_Surface *surface;
    
    // SDL Pixel Format Codes. These differ between platforms.
    uint8_t  Rshift, Gshift, Bshift;
    uint32_t Rmask, Gmask, Bmask;

	uint8_t palette[Video::S16_PALETTE_ENTRIES * 2]; // 2 Bytes Per Palette Entry
	uint32_t rgb[Video::S16_PALETTE_ENTRIES * 3];    // Extended to hold shadow/hilight colours

    // Video Mode
    uint8_t video_mode;

    // Enable Scanlines
    int scanlines;

    // Scanline pixels
    uint32_t* scan_pixels;

    // Original Screen Width & Height
    uint16_t orig_width, orig_height;

    // Screen Width/Height Of Window
    uint16_t screen_width, screen_height;

    // Scaled Width/Height
    uint32_t scaled_width, scaled_height;

    // Offsets (for full-screen mode, where x/y resolution isn't a multiple of the original height)
    uint32_t screen_xoff, screen_yoff;

    // Screen Scale multiplication factor
    uint8_t scale_factor;

	void refresh_palette(uint32_t);
    void scale( uint32_t* src, int srcwid, int srchgt, 
                uint32_t* dest, int dstwid, int dsthgt);
    void scanlines_32bpp(uint32_t* src, const int width, const int height, 
                         uint32_t* dst, int percent, const bool interpolate = true);

    void scalex( uint32_t* src, const int srcwid, const int srchgt, uint32_t* dest, const int scale);
};

extern Video video;