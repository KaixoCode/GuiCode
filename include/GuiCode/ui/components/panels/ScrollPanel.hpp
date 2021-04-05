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
template<typename PanelType>
class ScrollPanel : public Panel<>
{
public:
    /**
     * Constructor 
     */
    template<typename ...Args>
    ScrollPanel(Args ...args)
        : ::Panel<>(),
        m_Panel(Emplace<PanelType>(std::forward<Args>(args)...)),
        m_ScrollbarX(Emplace<Scrollbar<ScrollbarGraphics::Normal, ScrollbarType::Horizontal>>()),
        m_ScrollbarY(Emplace<Scrollbar<ScrollbarGraphics::Normal, ScrollbarType::Vertical>>())
    {
        m_Listener += [this](Event::MouseWheel& e)
        {
            if (e.wheelmod & Event::Mod::SHIFT || e.wheelmod & Event::Mod::CONTROL)
                return;

            if (m_Hovering && m_ScrollbarY.Visible())
                m_ScrollbarY.Scroll(-e.amount / 4);
        };

        m_Listener += [this](Event::MousePressed& e)
        {
            m_PMouseX = e.x;
            m_PMouseY = e.y;
            m_PValX = m_ScrollbarX.Value();
            m_PValY = m_ScrollbarY.Value();
            m_Dragging = true;
        };

        m_Listener += [this](Event::MouseReleased& e)
        {
            m_Dragging = false;
        };

        m_Listener += [this](Event::MouseEntered& e)
        {
            m_Hovering = true;
        };

        m_Listener += [this](Event::MouseExited& e)
        {
            m_Hovering = false;
        };

        m_Listener += [this](Event::MouseDragged& e)
        {
            if (m_Dragging && e.button == Event::MouseButton::MIDDLE)
            {
                if (m_ScrollbarY.Visible())
                    m_ScrollbarY.Value(m_PValY + e.y - m_PMouseY);
                if (m_ScrollbarX.Visible())
                    m_ScrollbarX.Value(m_PValX - e.x + m_PMouseX);
            }
        };
    }

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
    Vec2<int> Scrolled() const { return { m_ScrollbarX.Value(), m_ScrollbarY.Value() }; }

    /**
     * Retrieves the <code>Panel</code> inside this <code>ScrollPanel</code>
     * @return reference to the <code>Panel</code>
     */
    PanelType& Panel() const { return m_Panel; }

    void Update(const Vec4<int>& viewport) override
    {
        int yw = m_ScrollbarY.Width();
        int xh = m_ScrollbarX.Height();

        int yoff = m_ScrollbarX.NotNecessary() || !m_EnableX ? 0 : xh;
        m_ScrollbarY.Position({ Width() - yw, yoff });
        m_ScrollbarY.Height(Height() - yoff);
        m_ScrollbarY.Range({ 0, m_Panel.Height() + yoff });
        m_ScrollbarY.VisibleRange(Height());
        m_ScrollbarY.Visible(!m_ScrollbarY.NotNecessary() && m_EnableY);

        int xoff = m_ScrollbarY.NotNecessary() || !m_EnableY ? 0 : yw;
        m_ScrollbarX.Position({ 0, 0 });
        m_ScrollbarX.Width(Width() - xoff);
        m_ScrollbarX.Range({ 0, m_Panel.Width() + xoff });
        m_ScrollbarX.VisibleRange(Width());
        m_ScrollbarX.Visible(!m_ScrollbarX.NotNecessary() && m_EnableX);

        int _x = std::min(-m_ScrollbarX.Value(), 0);
        int _y = Height() - m_Panel.Height() + m_ScrollbarY.Value();
        if (m_ScrollbarY.NotNecessary())
            _y = Height() - m_Panel.Height();
        m_Panel.Position({ _x, _y });

        // If the panel is not being autoresized, set the width/height
        // to the size of this scroll panel.
        if (!m_EnableX)
            m_Panel.Width(Width() - xoff);

        if (!m_EnableY)
            m_Panel.Height(Height() - yoff);

        Panel::Update(viewport);

        int _nx = std::min(-m_ScrollbarX.Value(), 0);
        int _ny = Height() - m_Panel.Height() + m_ScrollbarY.Value();
        if (m_ScrollbarY.NotNecessary())
            _ny = Height() - m_Panel.Height();

        m_Panel.Position({ _nx, _ny });
    }

    void Render(CommandCollection& d) override
    {
        using namespace Graphics;
        d.Command<Clip>(X(), Y(), Width(), Height());
        d.Command<PushMatrix>();
        d.Command<Translate>(Vec2<int>{ X(), Y() });
        
        d.Command<Clip>(0, m_ScrollbarX.Height() * m_ScrollbarX.Visible(), Width() - m_ScrollbarY.Width() * m_ScrollbarY.Visible(), Height() - m_ScrollbarX.Height() * m_ScrollbarX.Visible());
        Background(d);
        d.Command<PopClip>();

        m_Panel.Render(d);
        d.Command<Clip>(0, 0, Width(), Height());
        if (m_ScrollbarX.Visible())
            m_ScrollbarX.Render(d);
        if (m_ScrollbarY.Visible())
            m_ScrollbarY.Render(d);

        if (m_ScrollbarX.Visible() && m_ScrollbarY.Visible())
        {
            int yw = m_ScrollbarY.Width();
            int xh = m_ScrollbarX.Height();
            d.Command<Fill>(Color{ 55, 55, 55, 0 });
            d.Command<Quad>(Width() - yw, 0, yw, xh);
        }
        d.Command<PopClip>();
        
        d.Command<PopMatrix>();
        d.Command<PopClip>();
        Component::Render(d);
    }

protected:
    PanelType& m_Panel{ 0 };
    ScrollbarBase& m_ScrollbarX{ 0 };
    ScrollbarBase& m_ScrollbarY{ 0 };

    bool m_EnableX = true,
        m_EnableY = true,
        m_Dragging = false,
        m_Hovering = false;

    int m_PMouseX = 0, 
        m_PMouseY = 0,
        m_PValX = 0,
        m_PValY = 0;
};
