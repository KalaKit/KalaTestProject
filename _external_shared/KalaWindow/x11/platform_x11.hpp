//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#ifdef KALAKIT_X11

#include <X11/Xlib.h>
#include <EGL/egl.h>

//kalawindow
#include "platform.hpp"

struct WINDOW
{
    ::Window handle;  //X11 native window handle
	Display* display; //X11 display server connection
};

typedef EGLContext OPENGLCONTEXT;

#endif // KALAKIT_X11