#pragma once
#include "GuiCode/ui/components/Container.hpp"
#include "GuiCode/ui/components/Layout.hpp"

// --------------------------------------------------------------------------
// -------------------------------- Panel -----------------------------------
// --------------------------------------------------------------------------

class Panel : public Container 
{
public:
    Panel(Vec2<int> size = { 40, 70 });

    auto Background(const Color& c)  -> void { m_Background = c; };
    auto TranslateBackground(bool t) -> void { m_TranslateBackground = t; }

    int Cursor() const override 
    { 
        auto a =
            !m_Pressed && m_LayoutManager.Cursor() != -1 ? m_LayoutManager.Cursor() :
            m_Pressed && m_Focussed ? m_Focussed->Cursor() : 
            m_Hovering ? m_Hovering->Cursor() : 
            m_Cursor; 
        return a;
    }

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;
    virtual void Background(CommandCollection& d);

    template<Layout::Type T, typename ...Args>
    void Layout(Args&& ...args) { m_LayoutManager.Layout<T>(std::forward<Args>(args)...); }

    void AutoResize(bool rx, bool ry) { m_AutoResizeX = rx, m_AutoResizeY = ry; };
    Vec2<bool> AutoResize() const { return { m_AutoResizeX, m_AutoResizeY }; };

protected:
    ::Color m_Background = { 0, 0, 0, 0 };
    LayoutManager m_LayoutManager;
    bool m_AutoResizeX = false, m_AutoResizeY = false, m_TranslateBackground = false;
};