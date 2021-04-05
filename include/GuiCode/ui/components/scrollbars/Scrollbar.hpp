#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/Component.hpp"

// --------------------------------------------------------------------------
// -------------------------- Scrollbar Base --------------------------------
// --------------------------------------------------------------------------

/**
 * Basis for <code>Scrollbar</code>
 */
class ScrollbarBase : public Component
{
public:
    /**
     * Constructor
     */
    ScrollbarBase();

    /**
     * Set the range of the <code>Scrollbar</code>.
     * @param r range
     */
    void Range(const Vec2<int>& r) { m_Range = r; ConstrainValue(); }

    /**
     * Scroll by <code>s</code> amount.
     * @param s amount
     */
    void Scroll(int s) { m_Value = constrain(s + m_Value, m_Range.start, m_Range.end - m_VisibleRange); }
    
    /**
     * Set absolute scroll value.
     * @param v value to scroll to
     */
    void Value(int v) { m_Value = constrain(v, m_Range.start, m_Range.end - m_VisibleRange); }
    
    /**
     * Set the visible range on the screen. This is used to calculate the size of the scrollbar.
     * @param v visible range
     */
    void VisibleRange(int v) { m_VisibleRange = v; ConstrainValue(); }

    /**
     * The minimun scrollbar size, this value is used to limit the scrollbar size to not become
     * infinitely small with big scrollareas.
     * @return minimum bar size
     */
    virtual int MinBarSize() const { return 40; }
    
    /**
     * Returns the value of this <code>Scrollbar</code>
     * @return value
     */
    int Value() const { return m_Value; }

    /**
     * This is used by the <code>ScrollbarGraphics</code> and returns the x or y 
     * (depending on what type of scrollbar it is) of the mouse when it started dragging the scrollbar.
     * @return x or y of mouse (depending on type of scrollbar)
     */
    int Mouse() const { return m_Mouse; }

    /**
     * Returns the visible range.
     * @return visible range
     */
    int VisibleRange() const { return m_VisibleRange; }

    /**
     * Returns the range.
     * @return range
     */
    Vec2<int> Range() const { return m_Range; }

    /**
     * Returns true when the scrollbar is not necessary, i.e. when the visible range is
     * lower than the range of the scrollbar.
     */
    bool NotNecessary() const { return m_VisibleRange >= m_Range.end - m_Range.start; }

    /**
     * Returns true when the mouse is hovering over the scrollbar.
     * @return true when mouse over scrollbar
     */
    virtual bool Hovering() const { return m_Hovering; }

    /**
     * Returns true when the mouse is dragging the scrollbar.
     * @return true when mouse dragging
     */
    virtual bool Dragging() const = 0;

    void Update(const Vec4<int>& viewport) override;

protected:
    Vec2<int> m_Range{0, 100};
    
    int m_VisibleRange = 20,
        m_Value = 0,
        m_Mouse = 0,
        m_PressMouse = 0,
        m_PressValue = 0;

    bool m_Hovering = false;

    void ConstrainValue() { m_Value = constrain(m_Value, m_Range.start, m_Range.end - m_VisibleRange); }
};

// --------------------------------------------------------------------------
// ----------------------------- Scrollbar ----------------------------------
// --------------------------------------------------------------------------

/**
 * Templated <code>Scrollbar</code> class, allows fully customized behaviour and graphics.
 * @tparam ScrollbarGraphics a class with a static Render method that takes in the <code>Scrollbar</code> and the <code>CommandCollection</code>.
 * @tparam ScrollbarType the type of scrollbar, must be a class that inherits ScrollbarBase.
 */
template<typename ScrollbarGraphics, typename ScrollbarType = ScrollbarBase>
class Scrollbar : public ScrollbarType
{
    using ScrollbarType::ScrollbarType;

    void Render(CommandCollection& d) override
    {
        ScrollbarType::Render(d);
        ScrollbarGraphics::Render(*this, d);
    };
};