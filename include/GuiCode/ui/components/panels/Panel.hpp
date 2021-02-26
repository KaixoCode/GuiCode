#pragma once
#include "GuiCode/ui/components/Container.hpp"
#include "GuiCode/ui/components/Layout.hpp"

// --------------------------------------------------------------------------
// -------------------------------- Panel -----------------------------------
// --------------------------------------------------------------------------

/**
 * A crucial building block, this component contains a layout which 
 * let's you arrange other panels or components.
 */
class Panel : public Container 
{
public:
    /**
     * Constructor
     * @param size size
     */
    Panel(Vec2<int> size = { 40, 70 });

    /**
     * A smart panel only redraws whenever a component inside this panel requests a redraw.
     * !!This feature is still a BETA and does not work entirely as intended yet!!
     * @param s enable smart panel
     */
    void SmartPanel(bool s) { m_SmartPanel = s; }
    bool SmartPanel() { return m_SmartPanel; }

    /**
     * Set the background color of this panel
     * @param c color
     */
    void Background(const Color& c) { m_Background = c; };

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

    /**
     * Overrideable method for rendering a background.
     * @param d the <code>CommandCollection</code> to append draw instructions to
     */
    virtual void Background(CommandCollection& d);

    /**
     * Set the layout of this <code>Panel</code>
     * @tparam T <code>Layout::Type</code>
     * @param ...args arguments for the layout type
     */
    template<Layout::Type T, typename ...Args>
    void Layout(Args&& ...args) { m_LayoutManager.Layout<T>(std::forward<Args>(args)...); }

    /**
     * Enable autoresizing of this panel in the x and y direction. When enabled, this Panel will
     * retrieve the highest x and y position from its <code>LayoutManager</code> and automatically
     * resize to that size.
     * @param rx auto resize in the x-axis
     * @param ty auto resize in the y-axis
     */
    void AutoResize(bool rx, bool ry) { m_AutoResizeX = rx, m_AutoResizeY = ry; };
    
    /**
     * Retrieve the current state of the AutoResize.
     * @return state of the AutoResize
     */
    Vec2<bool> AutoResize() const { return { m_AutoResizeX, m_AutoResizeY }; };

    bool NeedsRedraw() override { if (m_SmartPanel) return false; else return m_NeedsRedraw; }
    void NeedsRedraw(bool v) override { m_NeedsRedraw = v; }

protected:
    ::Color m_Background = { 0, 0, 0, 0 };
    LayoutManager m_LayoutManager;
    bool m_AutoResizeX = false, 
        m_AutoResizeY = false,
        m_SmartPanel = false;

    static inline unsigned int m_PanelCounter = 0;
    unsigned int m_PanelId = m_PanelCounter++;
};