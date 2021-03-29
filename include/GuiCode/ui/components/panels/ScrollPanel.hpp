#pragma once
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/scrollbars/Scrollbar.hpp"
#include "GuiCode/ui/components/scrollbars/ScrollbarType.hpp"
#include "GuiCode/ui/components/scrollbars/ScrollbarGraphics.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Scroll Panel --------------------------------
// --------------------------------------------------------------------------

/**
 * Simple scroll panel, contains another Panel which is translated.
 */
class ScrollPanel : public Panel
{
public:
    /**
     * Constructor 
     */
    ScrollPanel();

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;

    /**
     * Enable scrolling on the x and y axis.
     * @param x enable scroll in x-axis
     * @param y enable scroll in y-axis
     */
    void EnableScrollbars(bool x, bool y) { m_EnableX = x, m_EnableY = y; }

    /**
     * Returns the amount scrolled in both axes.
     * @return amount scrolled
     */
    Vec2<int> Scrolled() const { return { m_ScrollbarX->Value(), m_ScrollbarY->Value() }; }

    /**
     * Retrieves the <code>Panel</code> inside this <code>ScrollPanel</code>
     * @return reference to the <code>Panel</code>
     */
    ::Panel& Panel() const { return *m_Panel; }

    /**
     * Set the <code>Panel</code> in this <code>ScrollPanel</code>. This is templated to allow for different 
     * <code>Panel</code> types to be used inside a <code>ScrollPanel</code>.
     * @return reference to the emplaced panel
     * @tparam T panel type
     * @param ...args constructor arguments for <code>T</code>
     */
    template<typename T, typename ...Args>
    T& Panel(Args&&... args)
    {
        if (m_Panel != nullptr)
            return dynamic_cast<T&>(*m_Panel);
        
        auto& _t = Emplace<T>(std::forward<Args>(args)...);
        m_Panel = &_t;
        m_ScrollbarX = &Emplace<Scrollbar<ScrollbarGraphics::Normal, ScrollbarType::Horizontal>>();
        m_ScrollbarY = &Emplace<Scrollbar<ScrollbarGraphics::Normal, ScrollbarType::Vertical>>();
        return _t;
    }

protected:
    ScrollbarBase* m_ScrollbarX{ 0 };
    ScrollbarBase* m_ScrollbarY{ 0 };
    ::Panel* m_Panel{ 0 };

    bool m_EnableX = true,
        m_EnableY = true,
        m_Dragging = false,
        m_Hovering = false;

    int m_PMouseX = 0, 
        m_PMouseY = 0,
        m_PValX = 0,
        m_PValY = 0;
};
