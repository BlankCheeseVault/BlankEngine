#include <app.hpp>

void init()
{
}

void update()
{
}

void frame()
{
}

void event(const sapp_event *event)
{
}

void cleanup()
{
}

sapp_desc sokol_main(int argc, char *argv[])
{
    App::init_cb = init;
    App::update_cb = update;
    App::frame_cb = frame;
    App::event_cb = event;
    App::cleanup_cb = cleanup;
    return App::create("BlankEngine", 1920, 1080);
}