//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#ifdef KALAKIT_WINDOWS

#include <windows.h>
#include <GL/gl.h>

//kalawindow
#include "platform.hpp"

struct WINDOW
{
    HWND handle;      //Window handle
    HDC context;      //Device context
    WNDPROC callback; //Window procedure callback
};

typedef HGLRC OPENGLCONTEXT;

#endif // KALAKIT_WINDOWS