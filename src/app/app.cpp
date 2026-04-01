#include "app.hpp"

#ifdef WIN32
#include <windows.h>
#endif

void empty_void() {}
void empty_sapp_desc(sapp_desc *) {}
void empty_sapp_event(const sapp_event *) {}

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
}

void App::frame()
{
    update_cb();
    frame_cb();
}

void App::cleanup()
{
    cleanup_cb();
}

void App::event(const sapp_event *event)
{
    event_cb(event);
}
