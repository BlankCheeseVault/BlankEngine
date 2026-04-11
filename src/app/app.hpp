#pragma once

#include <string>
#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>

class App
{
public:
    static sg_pass screen_pass;
    static float debugScaleX;
    static float debugScaleY;

    static void (*create_cb)(sapp_desc *);
    static void (*init_cb)(void);
    static void (*update_cb)(void);
    static void (*frame_cb)(void);
    static void (*cleanup_cb)(void);
    static void (*event_cb)(const sapp_event *);

    static sapp_desc create(std::string title, int width, int height);

    static void debugPrint(const char *text, float x, float y, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);

private:
    static void init();
    static void frame();
    static void cleanup();
    static void event(const sapp_event *);
};