#pragma once
#include "GuiCode/pch.hpp"

// --------------------------------------------------------------------------
// ------------------------------ Window ------------------------------------
// --------------------------------------------------------------------------

#ifdef WIN32
#include "GuiCode/platform/windows/WindowsWindow.hpp"
using WindowType = WindowsWindow;
#endif

/**
 * Basic window, inherits a WindowType depending on OS.
 */
struct Window : public WindowType
{
    /**
     * Constructor
     * @param name name
     * @param width width
     * @param height height
     * @param resizable resizable
     * @param decorated decorated
     */
    Window(const std::string& name, int width, int height, bool resizable = true, bool decorated = true)
        : WindowType(name, width, height, resizable, decorated)
    {}
};