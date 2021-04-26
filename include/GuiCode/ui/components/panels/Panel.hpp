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
    Panel();

    /**
     * Set the background color of this panel
     * @param c color
     */
    void Background(const Color& c) { m_Background = c; };

    int Cursor() const override 
    { 
        auto a = m_LayoutManager.WantsCursor() ? m_LayoutManager.Cursor() :
            !m_Pressed && m_LayoutManager.Cursor() != -1 ? m_LayoutManager.Cursor() :
            m_Pressed && m_FocusedComponent ? m_FocusedComponent->Cursor() : 
            m_HoveringComponent ? m_HoveringComponent->Cursor() : 
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

    /**
     * Get the <code>LayoutManager</code> of this <code>Panel</code>
     * @return layout manager
     */
    LayoutManager& LayoutManager() { return m_LayoutManager; }

    /**
     * Get the <code>Div</code> of this <code>Panel</code>. Only used when the layout is 
     * set to <code>Layout::Divs</code>
     * @return div
     */
    Div& Div() { return m_LayoutManager.Div(); }

protected:
    ::Color m_Background = { 0, 0, 0, 0 };
    ::LayoutManager m_LayoutManager;
    bool m_AutoResizeX = false, 
        m_AutoResizeY = false;

    CommandCollection m_OldCollection;

    static inline unsigned int m_PanelCounter = 1;
    unsigned int m_PanelId = m_PanelCounter++;
};