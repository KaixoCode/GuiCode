#pragma once
#include "GuiCode/pch.hpp"

// --------------------------------------------------------------------------
// ------------------------------ Window ------------------------------------
// --------------------------------------------------------------------------

#ifdef WIN32
#include "GuiCode/platform/windows/WindowsWindow.hpp"
using WindowType = WindowsWindow;
#endif

struct Window : public WindowType
{
    Window(const std::string& name, int width, int height, bool child)
        : WindowType(name, width, height, child)
    {}
};