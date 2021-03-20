#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/Container.hpp"

/**
 * Base for a window
 */
class WindowBase : protected Container
{
public:
    /**
     * Constructor
     * @param name name
     * @param width width
     * @param height height
     */
    WindowBase(const std::string& name, int width, int height, bool hideonclose = false)
        : Container(), m_Name(name), m_HideOnClose(hideonclose)
    {
        m_Size = { width, height };
    }

    virtual auto Size(int w, int h)    -> void { Size({ w,h }); };
    virtual auto Size(Vec2<int> s)     -> void = 0;
    virtual auto Location(Vec2<int> s) -> void = 0;
    virtual auto Maximize()            -> void = 0;
    virtual auto Restore()             -> void = 0;
    virtual auto Iconify()             -> void = 0;
    virtual auto Close()               -> void = 0;
    virtual auto Show()                -> void = 0;
    virtual auto Hide()                -> void = 0;
    virtual auto Visible(bool b)       -> void { b ? Show() : Hide(); };
    virtual auto ShouldClose(bool b)   -> void = 0;
    virtual auto Loop()                -> void = 0;

    virtual Vec2<int> Size()        const = 0;
    virtual Vec2<int> Location()    const = 0;
    virtual Vec2<int> CursorPos()   const = 0;
    virtual int       Width()       const = 0;
    virtual int       Height()      const = 0;
    virtual bool      Maximized()   const = 0;
    virtual bool      Visible()     const = 0;
    virtual bool      ShouldClose() const = 0;
    virtual bool      HideOnClose() const { return m_HideOnClose; };


protected:
    std::string m_Name;
    bool m_HideOnClose = false;
};