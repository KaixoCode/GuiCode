#include "GuiCode/ui/components/panels/Panel.hpp"

// --------------------------------------------------------------------------
// -------------------------------- Panel -----------------------------------
// --------------------------------------------------------------------------

Panel::Panel()
    : Container()
{
    m_Listener += [this](Event& e)
    {
        // Make event coords relative to this panel
        if (e.type != Event::Type::KeyPressed && e.type != Event::Type::KeyReleased && e.type != Event::Type::KeyTyped)
            e.x -= X(), e.y -= Y();

        m_LayoutManager.AddEvent(e);
    };
}

void Panel::Background(CommandCollection& d)
{
    using namespace Graphics;
    d.Command<Fill>(m_Background);
    d.Command<Quad>(0, 0, Width(), Height());
};

void Panel::Update(const Vec4<int>& viewport)
{
    // Step 1 is updating the layout to make sure the positions of all components are correct.
    m_LayoutManager.Update({ 0, 0, Width(), Height() }, m_Components); // Also set the cursor
    m_Cursor = m_Pressed && m_LayoutManager.Cursor() == -1 ? GLFW_CURSOR_NORMAL : m_LayoutManager.Cursor();

    // Next: calculate the highest coords and auto resize to those coords if that is enabled.
    if (m_AutoResizeX)
        Width(m_LayoutManager.BiggestCoords().x);

    if (m_AutoResizeY)
        Height(m_LayoutManager.BiggestCoords().y);

    // Then update all the components in this container with the right viewport
    // make sure to overlap the viewport with the coords of this panel, and then
    // since all components in a panel are drawn relative from it, translate.
    Container::Update(viewport.Overlap({ X(), Y(), Width(), Height() }).Translate({ X(), Y() }));

    // If any of the components, in their update, changed size and requested a recalculation of
    // the positioning, do that.
    bool _recalc = false;
    for (auto& _c : m_Components)
        if (_c->NeedsRecalc())
            _recalc = true, _c->NeedsRecalc(false); 

    if (_recalc)
    {
        // Then this panel also requires a recalc
        m_NeedsRecalc = true;

        // Step 1 is updating the layout to make sure the positions of all components are correct.
        m_LayoutManager.Update({ 0, 0, Width(), Height() }, m_Components); // Also set the cursor
        m_Cursor = m_Pressed && m_LayoutManager.Cursor() == -1 ? GLFW_CURSOR_NORMAL : m_LayoutManager.Cursor();

        // Next: calculate the highest coords and auto resize to those coords if that is enabled.
        if (m_AutoResizeX)
            Width(m_LayoutManager.BiggestCoords().x);

        if (m_AutoResizeY)
            Height(m_LayoutManager.BiggestCoords().y);

        for (auto& _c : m_Components)
            _c->Update({ _c->Position(), _c->Size() });
    }
}

void Panel::Render(CommandCollection& d)
{
    using namespace Graphics;
    d.Command<PushMatrix>();

    d.Command<Translate>(Vec2<int>{ X(), Y() });
    Background(d);
    Vec2<int> m_Pos = Position();
    Position({ 0,0 });
    Container::Render(d);
    Position(m_Pos);
    m_LayoutManager.DrawDividers(d);
    d.Command<PopMatrix>();
}