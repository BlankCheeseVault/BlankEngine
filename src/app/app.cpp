#include "app.hpp"

#ifdef WIN32
#include <windows.h>
#endif

#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_glue.h>
#include <sokol/sokol_log.h>
#include <sokol/util/sokol_debugtext.h>

void empty_void() {}
void empty_sapp_desc(sapp_desc *) {}
void empty_sapp_event(const sapp_event *) {}

sg_pass App::screen_pass = {};
float App::debugScaleX = 1;
float App::debugScaleY = 1;

void (*App::create_cb)(sapp_desc *) = empty_sapp_desc;
void (*App::init_cb)(void) = empty_void;
void (*App::update_cb)(void) = empty_void;
void (*App::frame_cb)(void) = empty_void;
void (*App::cleanup_cb)(void) = empty_void;
void (*App::event_cb)(const sapp_event *) = empty_sapp_event;

sapp_desc App::create(std::string title, int width, int height)
{
    sapp_desc desc = {};

    desc.init_cb = App::init;
    desc.frame_cb = App::frame;
    desc.event_cb = App::event;
    desc.cleanup_cb = App::cleanup;

    desc.window_title = title.c_str();
    desc.width = width;
    desc.height = height;
    desc.logger.func = slog_func;

#ifdef WIN32
    // https://stackoverflow.com/a/54912192

    HWND activeWindow = GetActiveWindow();
    HMONITOR monitor = MonitorFromWindow(activeWindow, MONITOR_DEFAULTTONEAREST);

    // Get the logical width and height of the monitor
    MONITORINFOEX monitorInfoEx;
    monitorInfoEx.cbSize = sizeof(monitorInfoEx);
    GetMonitorInfo(monitor, &monitorInfoEx);
    auto cxLogical = monitorInfoEx.rcMonitor.right - monitorInfoEx.rcMonitor.left;
    auto cyLogical = monitorInfoEx.rcMonitor.bottom - monitorInfoEx.rcMonitor.top;

    // Get the physical width and height of the monitor
    DEVMODE devMode;
    devMode.dmSize = sizeof(devMode);
    devMode.dmDriverExtra = 0;
    EnumDisplaySettings(monitorInfoEx.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
    auto cxPhysical = devMode.dmPelsWidth;
    auto cyPhysical = devMode.dmPelsHeight;

    // Calculate the scaling factor
    auto horizontalScale = ((double)cxPhysical / (double)cxLogical);
    auto verticalScale = ((double)cyPhysical / (double)cyLogical);

    desc.width = desc.width / horizontalScale;
    desc.height = desc.height / verticalScale;
#endif

    App::create_cb(&desc);

    return desc;
}

void App::init()
{
#ifdef WIN32
    HWND hwnd = (HWND)sapp_win32_get_hwnd();
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int win_w = rect.right - rect.left;
    int win_h = rect.bottom - rect.top;

    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);

    int x = (screen_w - win_w) / 2;
    int y = (screen_h - win_h) / 2;

    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
#endif

    { // graphics
        sg_desc desc = {};
        desc.environment = sglue_environment();
        desc.logger.func = slog_func;
        sg_setup(desc);
    }

    { // debug text
        sdtx_desc_t desc = {};
        desc.fonts[0] = sdtx_font_kc853();
        desc.logger.func = slog_func;
        sdtx_setup(desc);
    }

    screen_pass.action.colors[0].load_action = SG_LOADACTION_CLEAR;
    screen_pass.action.colors[0].clear_value = {0, 0, 0, 1};
    screen_pass.action.depth.load_action = SG_LOADACTION_CLEAR;
    screen_pass.action.depth.clear_value = 1;

    init_cb();
}

void App::frame()
{
    update_cb();
    
    screen_pass.swapchain = sglue_swapchain();
    sg_begin_pass(&screen_pass);

    sdtx_canvas(sapp_widthf() / debugScaleX, sapp_heightf() / debugScaleY);
    sdtx_origin(2.0f, 2.0f);

    frame_cb();

    sdtx_draw();

    sg_end_pass();
    sg_commit();
}

void App::cleanup()
{
    cleanup_cb();
}

void App::event(const sapp_event *event)
{
    event_cb(event);
}

void App::debugPrint(const char *text, float x, float y, uint8_t r, uint8_t g, uint8_t b)
{
    sdtx_home();
    sdtx_move(x, y);
    sdtx_font(0);
    sdtx_color3b(r, g, b);
    sdtx_puts(text);
    sdtx_crlf();
}
