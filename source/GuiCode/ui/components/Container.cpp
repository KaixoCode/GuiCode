#include "GuiCode/ui/components/Container.hpp"

Container::Container()
    : Component()
{
    // General event listener to forward the events to the appropriate components
    // inside this container.
    m_Listener += [this](Event& e)
    {
        if (e.type == Event::Type::MousePressed)
            m_Pressed = true;

        if (e.type == Event::Type::MouseReleased)
            m_Pressed = false;

        Event _copy = e;
        Event _copy2 = e;

        if (e.type == Event::Type::KeyPressed || e.type == Event::Type::KeyReleased || e.type == Event::Type::KeyTyped)
        {
            for (auto& _c : m_Components)
                _c->AddEvent(_copy);

            return;
        }

        // If there was an unfocus event, unfocus the focussed component
        if (m_FocusedComponent && e.type == Event::Type::Unfocused)
        {
            m_FocusedComponent->AddEvent(_copy);
            return;
        }

        if (e.type == Event::Type::MouseEntered)
        {
            if (m_HoveringComponent) 
                m_HoveringComponent->AddEvent(_copy);
            
            return;
        }

        // If the mouse moved or pressed, update the hovering and focussed
        if (e.type == Event::Type::MouseDragged || e.type == Event::Type::MouseMoved || e.type == Event::Type::MousePressed)
            this->Determine(e);

        // Send events to hovering and focussed if it's not the same component.
        if (m_HoveringComponent) 
            m_HoveringComponent->AddEvent(_copy);
        
        if (m_HoveringComponent != m_FocusedComponent && m_FocusedComponent) 
            m_FocusedComponent->AddEvent(_copy2);
        
        // If the mouse is released update the hovering and focussed after sending the events.
        if (e.type == Event::Type::MouseReleased)
            this->Determine(e);
    };
}

void Container::Update(const Vec4<int>& viewport)
{
    // This just sets the m_Viewport to viewport.
    Component::Update(viewport);
    
    // Only update if the viewport has actual size.
    if (m_Viewport.width == 0 || m_Viewport.height == 0)
        return;
    
    // Update all the components that lie within the viewport and are visible.
    for (auto& _c : m_Components)
    {
        if (_c->Visible() &&
            _c->X() + _c->Width() >= m_Viewport.x && _c->Y() + _c->Height() >= m_Viewport.y &&
            _c->X() <= m_Viewport.x + m_Viewport.width && _c->Y() <= m_Viewport.y + m_Viewport.height)
        {
            if (_c->NeedsRedraw())
                m_NeedsRedraw = true;

            _c->Update(viewport);
        }

        // If a sub component gained focus on its own, also focus this container.
        if (m_FocusedComponent == nullptr && _c->Focused())
            m_FocusedComponent = _c.get(), Focused(true);
    }

    // If the focused component lost focus, check if it swapped to other component, otherwise
    // remove focus from this container as well.
    if (m_FocusedComponent && !m_FocusedComponent->Focused())
    {
        m_FocusedComponent = nullptr;

        bool _found = false;
        for (auto& _c : m_Components)
            if (_c->Focused())
                m_FocusedComponent = _c.get(), Focused(true), _found = true;

        if (!_found)
            Focused(false), m_FocusedComponent = nullptr;
    }
}

void Container::Render(CommandCollection& d)
{
    Component::Render(d);

    // Only render if the viewport has actual size.
    if (m_Viewport.width == 0 || m_Viewport.height == 0)
        return;

    // Render all the components that lie within the viewport and are visible.
    for (auto& _c : m_Components)
        if (_c->Visible() &&
            _c->X() + _c->Width() >= m_Viewport.x && _c->Y() + _c->Height() >= m_Viewport.y &&
            _c->X() <= m_Viewport.x + m_Viewport.width && _c->Y() <= m_Viewport.y + m_Viewport.height)
        {
            _c->Render(d);
        }
}

void Container::Determine(Event& e)
{
    // Determine the next hovering
    Component* _nextHover = 0;
    for (auto& _c : m_Components)
        if (_c->Visible())
            if (_c->WithinBounds({ e.x, e.y }))
                _nextHover = _c.get();

    // If it is different, send MouseEntered and MouseExited events.
    if (_nextHover != m_HoveringComponent)
    {
        if (m_HoveringComponent)
        {
            Event _e{ Event::Type::MouseExited, e.x, e.y };
            m_HoveringComponent->AddEvent(_e);
            m_HoveringComponent->Hovering(false);
        }

        if (_nextHover != nullptr)
        {
            Event _e{ Event::Type::MouseEntered, e.x, e.y };
            _nextHover->AddEvent(_e);
            _nextHover->Hovering(true);
        }
        m_HoveringComponent = _nextHover;
    }

    // If MousePressed and Hovering is not focussed, send focus events.
    if (e.type == Event::Type::MousePressed && m_HoveringComponent != m_FocusedComponent)
    {
        if (m_FocusedComponent != nullptr)
        {
            Event _e{ Event::Type::Unfocused };
            m_FocusedComponent->Focused(false);
            m_FocusedComponent->AddEvent(_e);
        }

        m_FocusedComponent = m_HoveringComponent;
        if (m_FocusedComponent != nullptr)
        {
            Event _e{ Event::Type::Focused };
            m_FocusedComponent->Focused(true);
            m_FocusedComponent->AddEvent(_e);
        }
    }
}

void Container::Focused(bool v)
{
    m_Focused = v;

    // Make sure all sub components are also unfocused.
    if (!v)
        for (auto& _c : m_Components)
            _c->Focused(false);
}

void Container::Hovering(bool v)
{
    m_Hovering = v;

    // Make sure all sub components are also unfocused.
    if (!v)
        for (auto& _c : m_Components)
            _c->Hovering(false);
}

void Container::Remove(int index)
{
    // Make sure hovering and focussed dont point to nothing
    auto& a = m_Components.at(index);
    if (a.get() == m_HoveringComponent)
        m_HoveringComponent = nullptr;

    if (a.get() == m_FocusedComponent)
        m_FocusedComponent = nullptr;

    m_Components.erase(m_Components.begin() + index);
}