#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/graphics/Collection.hpp"
#include "GuiCode/ui/events/EventListener.hpp"
#include "GuiCode/ui/components/Layout.hpp"

class Component
{
public:
    Component()
    {}

    Component(Vec2<int> size)
        : m_Size(size)
    {}

    virtual ~Component()
    {}

    virtual auto Position(const Vec2<int>& pos) -> void { m_Pos = pos; }
    virtual auto Size(const Vec2<int>& size)    -> void { m_Size = size; }
    virtual auto MinSize(const Vec2<int>& size) -> void { m_MinSize = size; }
    virtual auto MaxSize(const Vec2<int>& size) -> void { m_MaxSize = size; }
    virtual auto Width(int w)                   -> void { m_Size = { w, m_Size.height }; }
    virtual auto Height(int h)                  -> void { m_Size = { m_Size.width, h }; }
    virtual auto MinWidth(int w)                -> void { m_MinSize = { w, m_MinSize.height }; }
    virtual auto MinHeight(int h)               -> void { m_MinSize = { m_MinSize.width, h }; }
    virtual auto MaxWidth(int w)                -> void { m_MaxSize = { w, m_MaxSize.height }; }
    virtual auto MaxHeight(int h)               -> void { m_MaxSize = { m_MaxSize.width, h }; }
    virtual auto Hide()                         -> void { m_Visible = false; }
    virtual auto Show()                         -> void { m_Visible = true; }
    virtual auto Visible(bool v)                -> void { m_Visible = v; }
    virtual auto LayoutHint(Layout::Hint h)     -> void { m_LayoutHint = h; }
    virtual auto Viewport(const Vec4<int>& v)   -> void { m_Viewport = v; }

    virtual Vec2<int>    Position()   const { return m_Pos; }
    virtual int          X()          const { return m_Pos.x; }
    virtual int          Y()          const { return m_Pos.y; }
    virtual Vec2<int>    Size()       const { return m_Size; }
    virtual Vec2<int>    MinSize()    const { return m_MinSize; }
    virtual Vec2<int>    MaxSize()    const { return m_MaxSize; }
    virtual int          Height()     const { return m_Size.height; }
    virtual int          Width()      const { return m_Size.width; }
    virtual int          MinHeight()  const { return m_MinSize.height; }
    virtual int          MinWidth()   const { return m_MinSize.width; }
    virtual int          MaxHeight()  const { return m_MaxSize.height; }
    virtual int          MaxWidth()   const { return m_MaxSize.width; }
    virtual int          Cursor()     const { return m_Cursor; }
    virtual bool         Visible()    const { return m_Visible; }
    virtual Layout::Hint LayoutHint() const { return m_LayoutHint; }

    virtual bool WithinBounds(const Vec2<int>& pos) const;

    virtual void Render(CommandCollection& d) { }
    virtual void Update(const Vec4<int>& viewport);

    void AddEvent(Event& e) { m_Listener.AddEvent(e); }

protected:
    Vec4<int> m_Viewport{ -1, -1, -1, -1 };

    Vec2<int> m_Pos{ 0, 0 };
    Vec2<int> m_Size{ 1, 1 };
    Vec2<int> m_MinSize{ 10, 10 };
    Vec2<int> m_MaxSize{ -1, -1 };

    Layout::Hint m_LayoutHint = Layout::Hint::None;

    int m_Cursor = GLFW_CURSOR_NORMAL;

    bool m_Visible = true;

    EventListener m_Listener;
};

using ComponentCollection = std::vector<std::unique_ptr<Component>>;
