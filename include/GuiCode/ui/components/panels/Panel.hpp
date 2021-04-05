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
template<typename Type = Component, typename Collection = OwningCollection<Type>>
class Panel : public Container<Type, Collection>
{
public:
    /**
     * Constructor
     * @param size size
     */
    Panel()
        : Container<Type, Collection>()
    {
        m_Listener += [this](Event& e)
        {
            // Make event coords relative to this panel
            if (e.type != Event::Type::KeyPressed && e.type != Event::Type::KeyReleased && e.type != Event::Type::KeyTyped)
                e.x -= X(), e.y -= Y();

            m_LayoutManager.AddEvent(e);
        };

        // TODO: Find a better fix than just updating when mousepressed....
        // Hacky fix to the TreePanel Folder expanding bug...
        m_Listener += [this](Event::MousePressed& e)
        {
            Container::Update(m_Viewport);
        };
    }

    /**
     * Set the background color of this panel
     * @param c color
     */
    void Background(const Color& c) { m_Background = c; };

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

    /**
     * Overrideable method for rendering a background.
     * @param d the <code>CommandCollection</code> to append draw instructions to
     */
    virtual void Background(CommandCollection& d)
    {
        using namespace Graphics;
        d.Command<Fill>(m_Background);
        d.Command<Quad>(0, 0, Width(), Height());
    };

    int Cursor() const override
    {
        auto a = m_LayoutManager.WantsCursor() ? m_LayoutManager.Cursor() :
            !m_Pressed && m_LayoutManager.Cursor() != -1 ? m_LayoutManager.Cursor() :
            m_Pressed && m_FocusedComponent ? m_FocusedComponent->Cursor() :
            m_HoveringComponent ? m_HoveringComponent->Cursor() :
            m_Cursor;
        return a;
    }

    void Update(const Vec4<int>& viewport)
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
    }

    void Render(CommandCollection& d)
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


protected:
    ::Color m_Background = { 0, 0, 0, 0 };
    ::LayoutManager m_LayoutManager;
    bool m_AutoResizeX = false,
        m_AutoResizeY = false;

    static inline unsigned int m_PanelCounter = 0;
    unsigned int m_PanelId = m_PanelCounter++;
};