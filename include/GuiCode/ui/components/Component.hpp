#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/graphics/Collection.hpp"
#include "GuiCode/ui/events/EventListener.hpp"
#include "GuiCode/ui/components/Layout.hpp"

class Component
{
public:

    /**
     * Constructor
     */
    Component()
    {}

    /**
     * Constructor
     * @param size size
     */
    Component(Vec2<int> size)
        : m_Size(size)
    {}

    virtual ~Component()
    {}

    /**
     * Set the position of the <code>Component</code>.
     * @param pos the position
     */
    virtual void Position(const Vec2<int>& pos) { m_Pos = pos; }

    /**
     * Set the size of the <code>Component</code>.
     * @param size the size
     */
    virtual void Size(const Vec2<int>& size) { m_Size = size; }

    /**
     * Set the minimum size of the <code>Component</code>.
     * @param size the minimum size
     */
    virtual void MinSize(const Vec2<int>& size) { m_MinSize = size; }

    /**
     * Set the maximum size of the <code>Component</code>.
     * @param size the maximum size
     */
    virtual void MaxSize(const Vec2<int>& size) { m_MaxSize = size; }
    
    /**
     * Set the width of the <code>Component</code>.
     * @param w the width
     */
    virtual void Width(int w) { m_Size = { w, m_Size.height }; }
    
    /**
     * Set the height of the <code>Component</code>.
     * @param h the height
     */
    virtual void Height(int h) { m_Size = { m_Size.width, h }; }
    
    /**
     * Set the minimum width of the <code>Component</code>.
     * @param w the minimum width
     */
    virtual void MinWidth(int w) { m_MinSize = { w, m_MinSize.height }; }
   
    /**
     * Set the minimum height of the <code>Component</code>.
     * @param h the minimum height
     */
    virtual void MinHeight(int h) { m_MinSize = { m_MinSize.width, h }; }
    
    /**
     * Set the maximum width of the <code>Component</code>.
     * @param w the maximum width
     */
    virtual void MaxWidth(int w) { m_MaxSize = { w, m_MaxSize.height }; }

    /**
     * Set the maximum height of the <code>Component</code>.
     * @param h the maximum height
     */
    virtual void MaxHeight(int h) { m_MaxSize = { m_MaxSize.width, h }; }
    
    /**
     * Hide the <code>Component</code>; same as <code> Visible(false) </code>.
     */
    virtual void Hide() { Visible(false); }

    /**
     * Show the <code>Component</code>; same as <code> Visible(true) </code>.
     */
    virtual void Show() { Visible(true); }
    
    /**
     * Set the visibility of the <code>Component</code>.
     */
    virtual void Visible(bool v) { m_Visible = v; }

    /**
     * Set a <code>Layout::Hint</code> for the <code>Component</code>.
     */
    virtual void LayoutHint(Layout::Hint h) { m_LayoutHint = h; }
    
    /**
     * Set the viewport of the <code>Component</code>.
     */
    virtual void Viewport(const Vec4<int>& v) { m_Viewport = v; }

    /**
     * Get the position of the <code>Component</code> 
     * @return position
     */
    virtual Vec2<int> Position() const { return m_Pos; }

    /**
     * Get the x-position of the <code>Component</code>
     * @return x-position
     */
    virtual int X() const { return m_Pos.x; }

    /**
     * Get the y-position of the <code>Component</code>
     * @return y-position
     */
    virtual int Y() const { return m_Pos.y; }
    
    /**
     * Get the size of the <code>Component</code>
     * @return size
     */
    virtual Vec2<int> Size() const { return m_Size; }
    
    /**
    * Get the minimum size of the <code>Component</code>
    * @return minimum size
    */
    virtual Vec2<int> MinSize() const { return m_MinSize; }

    /**
     * Get the maximum size of the <code>Component</code>
     * @return maximum size
     */
    virtual Vec2<int> MaxSize() const { return m_MaxSize; }

    /**
     * Get the height of the <code>Component</code>
     * @return height
     */
    virtual int Height() const { return m_Size.height; }

    /**
     * Get the width of the <code>Component</code>
     * @return width
     */
    virtual int Width() const { return m_Size.width; }

    /**
     * Get the minimum height of the <code>Component</code>
     * @return minimum height
     */
    virtual int MinHeight() const { return m_MinSize.height; }

    /**
     * Get the minimum width of the <code>Component</code>
     * @return minimum width
     */
    virtual int MinWidth() const { return m_MinSize.width; }

    /**
     * Get the maximum height of the <code>Component</code>
     * @return maximum height
     */
    virtual int MaxHeight() const { return m_MaxSize.height; }

    /**
     * Get the maximum width of the <code>Component</code>
     * @return maximum width
     */
    virtual int MaxWidth() const { return m_MaxSize.width; }

    /**
     * Get the cursor that will be used when hovering over the <code>Component</code>
     * @return cursor id
     */
    virtual int Cursor() const { return m_Cursor; }

    /**
     * Get current visibility of the <code>Component</code>
     * @return current visibility
     */
    virtual bool Visible() const { return m_Visible; }

    /**
     * Get the <code>Layout::Hint</code> of the <code>Component</code>
     * @return <code>Layout::Hint</code>
     */
    virtual Layout::Hint LayoutHint() const { return m_LayoutHint; }
    
    /**
     * Returns true when this <code>Component</code> needs a redraw
     * @return true when a redraw is needed
     */
    virtual bool NeedsRedraw() { return m_NeedsRedraw; }

    /**
     * Returns true when the given <code>pos</code> is inside the <code>Component</code>
     * @return true when <code>pos</code> is inside of the bounds of this <code>Component</code>
     * @param pos position to check
     */
    virtual bool WithinBounds(const Vec2<int>& pos) const;

    /**
     * Render the <code>Component</code>
     * @param d the <code>CommandCollection</code> to append draw instructions to
     */
    virtual void Render(CommandCollection& d) { m_NeedsRedraw = false; }
    
    /**
     * Update the <code>Component</code>
     * @param viewport the viewport
     */
    virtual void Update(const Vec4<int>& viewport);

    /**
     * Add an event to this <code>Component</code>
     * @param e an <code>Event</code>
     */
    void AddEvent(Event& e) { m_Listener.AddEvent(e); }

protected:
    Vec4<int> m_Viewport{ -1, -1, -1, -1 };

    Vec2<int> m_Pos{ 0, 0 };
    Vec2<int> m_Size{ 1, 1 };
    Vec2<int> m_MinSize{ 10, 10 };
    Vec2<int> m_MaxSize{ -1, -1 };

    Layout::Hint m_LayoutHint = Layout::Hint::None;

    int m_Cursor = GLFW_CURSOR_NORMAL;

    bool m_Visible = true,
        m_NeedsRedraw = true;
    
    EventListener m_Listener;
};

using ComponentCollection = std::vector<std::unique_ptr<Component>>;
