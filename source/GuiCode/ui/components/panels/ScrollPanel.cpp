#include "GuiCode/ui/components/panels/ScrollPanel.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Scroll Panel --------------------------------
// --------------------------------------------------------------------------

ScrollPanel::ScrollPanel()
    : ::Panel()
{
    m_Listener += [this](Event::MouseWheel& e)
    {
        if (e.wheelmod & Event::Mod::SHIFT || e.wheelmod & Event::Mod::CONTROL)
            return;

        // Don't scroll if hovering over child scrollpanel
        if (HoveringOverChildScrollPanel())
            return;

        if (m_Hovering && m_ScrollbarY->Visible())
            m_ScrollbarY->Scroll(-e.amount / 4);
    };

    m_Listener += [this](Event::MousePressed& e)
    {
        m_PMouseX = e.x;
        m_PMouseY = e.y;
        m_PValX = m_ScrollbarX->Value();
        m_PValY = m_ScrollbarY->Value();
        
        // Don't start dragging if hovering over child scrollpanel
        if (HoveringOverChildScrollPanel())
            return;

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
            if (m_ScrollbarY->Visible())
                m_ScrollbarY->Value(m_PValY + e.y - m_PMouseY);
            if (m_ScrollbarX->Visible())
                m_ScrollbarX->Value(m_PValX - e.x + m_PMouseX);
        }
    };
}

void ScrollPanel::Update(const Vec4<int>& viewport) 
{
    if (m_Panel)
    {
        int yw = m_ScrollbarY->Width();
        int xh = m_ScrollbarX->Height();

        bool scrollxenable = !m_ScrollbarX->NotNecessary() && (m_EnableX || m_Panel->Width() > Width());
        bool scrollyenable = !m_ScrollbarY->NotNecessary() && (m_EnableY || m_Panel->Height() > Height());
        int yoff = scrollxenable ? xh : 0;
        m_ScrollbarY->Position({ Width() - yw, yoff });
        m_ScrollbarY->Height(Height() - yoff);
        m_ScrollbarY->Range({ 0, m_Panel->Height() + yoff });
        m_ScrollbarY->VisibleRange(Height());
        m_ScrollbarY->Visible(scrollyenable);

        int xoff = scrollyenable ? yw : 0;
        m_ScrollbarX->Position({ 0, 0 });
        m_ScrollbarX->Width(Width() - xoff);
        m_ScrollbarX->Range({ 0, m_Panel->Width() + xoff });
        m_ScrollbarX->VisibleRange(Width());
        m_ScrollbarX->Visible(scrollxenable);

        int _x = std::min(-m_ScrollbarX->Value(), 0);
        int _y = Height() - m_Panel->Height() + m_ScrollbarY->Value();
        if (m_ScrollbarY->NotNecessary())
            _y = Height() - m_Panel->Height();
        m_Panel->Position({ _x, _y });

        // If the panel is not being autoresized, set the width/height
        // to the size of this scroll panel.
        if (!m_EnableX)
            m_Panel->Width(Width() - xoff);

        if (!m_EnableY)
            m_Panel->Height(Height() - yoff);

        m_Panel->ConstrainSize();
        
        Panel::Update(viewport);

        int _nx = std::min(-m_ScrollbarX->Value(), 0);
        int _ny = Height() - m_Panel->Height() + m_ScrollbarY->Value();
        if (m_ScrollbarY->NotNecessary())
            _ny = Height() - m_Panel->Height();

        m_Panel->Position({ _nx, _ny });
    }
}

void ScrollPanel::Render(CommandCollection& d) 
{
    using namespace Graphics;
    d.Command<Clip>(X(), Y(), Width(), Height());
    d.Command<PushMatrix>();
    d.Command<Translate>(Vec2<int>{ X(), Y() });
    if (m_Panel)
    {
        d.Command<Clip>(0, m_ScrollbarX->Height() * m_ScrollbarX->Visible(), Width() - m_ScrollbarY->Width() * m_ScrollbarY->Visible(), Height() - m_ScrollbarX->Height() * m_ScrollbarX->Visible());
        Background(d);
        d.Command<PopClip>();

        m_Panel->Render(d);
        d.Command<Clip>(0, 0, Width(), Height());
        if (m_ScrollbarX->Visible())
            m_ScrollbarX->Render(d);
        if (m_ScrollbarY->Visible())
            m_ScrollbarY->Render(d);

        if (m_ScrollbarX->Visible() && m_ScrollbarY->Visible())
        {
            int yw = m_ScrollbarY->Width();
            int xh = m_ScrollbarX->Height();
            d.Command<Fill>(Color{ 55, 55, 55, 0 });
            d.Command<Quad>(Width() - yw, 0, yw, xh);
        }
        d.Command<PopClip>();
    }
    d.Command<PopMatrix>();
    d.Command<PopClip>();
    Component::Render(d);
    NeedsRedraw(false);
}

bool ScrollPanel::HoveringOverChildScrollPanel()
{
    if (m_HoveringComponent != nullptr)
    {
        ScrollPanel* _scroll = nullptr;
        Container* _cont = nullptr;
        Component* _comp = m_HoveringComponent;

        // Go through all hovering components until we
        // reach a point when it's no longer a container
        while (_cont = dynamic_cast<Container*>(_comp))
        {
            _comp = _cont->HoveringComponent();

            // See if the container is a scrollpanel, if it is, it
            // means we have a child scrollpanel that we're hovering
            // over, so we don't want to scroll this scrollpanel, so
            // we return.
            if (dynamic_cast<ScrollPanel*>(_cont))
                return true;
        }
    }
    return false;
}