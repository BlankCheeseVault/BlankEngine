#pragma once

#include <string>
#include <sokol/sokol_app.h>

class App
{
public:
    static void (*create_cb)(sapp_desc *);
    static void (*init_cb)(void);
    static void (*update_cb)(void);
    static void (*frame_cb)(void);
    static void (*cleanup_cb)(void);
    static void (*event_cb)(const sapp_event *);

    static sapp_desc create(std::string title, int width, int height);

private:
    static void init();
    static void frame();
    static void cleanup();
    static void event(const sapp_event *);
};