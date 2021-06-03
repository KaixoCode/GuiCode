#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/Container.hpp"

namespace PlatformFunctions
{
    Vec2<int> GlobalCursorPos();
    Vec4<int> MonitorRecFromPoint(Vec2<int> point);
}

class WindowBase;

struct WindowData
{
    WindowData() {}
    WindowData(const std::string& n, Vec2<int> s, bool a, bool t, bool h, bool c, bool r, bool d, WindowBase* p)
        : name(n), size(s), alwaysOnTop(a), transparentBuffer(t), hideOnClose(h), showOnCreate(c), resizeable(r),
        decorated(d), parent(p)
    {}

    std::string name;
    Vec2<int> size;
    bool alwaysOnTop = false;
    bool transparentBuffer = false;
    bool hideOnClose = false;
    bool showOnCreate = true;
    bool resizeable = true;
    bool decorated = true;
    WindowBase* parent = nullptr;
};
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
    WindowBase(const WindowData& d)
        : Container(), m_Name(d.name), m_HideOnClose(d.hideOnClose)
    {
        m_Size = d.size;
    }

    virtual auto Size(int w, int h)    -> void { Size({ w,h }); };
    virtual auto Size(Vec2<int> s)     -> void = 0;
    virtual auto MaxSize(int w, int h) -> void = 0;
    virtual auto MaxSize(Vec2<int> s)  -> void = 0;
    virtual auto MinSize(int w, int h) -> void = 0;
    virtual auto MinSize(Vec2<int> s)  -> void = 0;
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
    virtual auto Scale(float)          -> void = 0;
    virtual auto AlwaysOnTop(bool)     -> void = 0;
    virtual auto ToForeground()        -> void = 0;

    virtual Vec2<int> Size()                const = 0;
    virtual Vec2<int> Location()            const = 0;
    virtual Vec2<int> CursorPos()           const = 0;
    virtual Vec4<int> GetWindowRect()       const = 0;
    virtual int       Width()               const = 0;
    virtual int       Height()              const = 0;
    virtual bool      Maximized()           const = 0;
    virtual bool      Visible()             const = 0;
    virtual bool      ShouldClose()         const = 0;
    virtual bool      HideOnClose()         const { return m_HideOnClose; };
    virtual bool      IsForegroundWindow()  const = 0;


protected:
    std::string m_Name;
    bool m_HideOnClose = false;
};