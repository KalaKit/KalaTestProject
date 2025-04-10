//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#ifdef KALAKIT_WAYLAND

#include <wayland-client.h>
#include <EGL/egl.h>

//kalawindow
#include "platform.hpp"

struct WINDOW
{
    struct wl_surface* handle;          //Wayland native window handle
    struct wl_display* display;         //Wayland display server connection
    struct wl_egl_window* renderTarget; //EGL surface render wrapper
};

typedef EGLContext OPENGLCONTEXT;

#endif // KALAKIT_WAYLAND