#pragma once
#include "GuiCode/ui/components/Component.hpp"
#include "GuiCode/ui/components/LayoutManager.hpp"
#include "GuiCode/ui/components/Layout.hpp"

template<typename Type>
struct unique_wrapper : public std::unique_ptr<Type>
{
    using unique_ptr<Type>::unique_ptr;

    operator Type* () { return get(); }
};

template<typename Type = Component, typename Collection = std::vector<unique_wrapper<Type>>, typename = typename std::enable_if_t<std::is_base_of_v<Component, Type>>>
class Container : public Component
{
public:

    Container() 
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
    };

    /**
     * Emplace a component to this container,
     * @return reference to the emplaced component.
     * @tparam T type
     * @param ...args constructor arguments for <code>T</code>
     */
    template<typename T, typename ...Args>
    T& Add(T* a)
    {
        m_Components.push_back(a);
        return *a;
    }

    /**
     * Emplace a component to this container,
     * @return reference to the emplaced component.
     * @tparam T type
     * @param ...args constructor arguments for <code>T</code>
     */
    template<typename T, typename ...Args>
    T& Emplace(Args&&... args)
    {
        auto& _t = m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        return *dynamic_cast<T*>((Type*)_t);
    }

    /**
     * Emplace a component to this container,
     * @return reference to the emplaced component.
     * @tparam T type
     * @param h layout hint for the component
     * @param ...args constructor arguments for <code>T</code>
     */
    template<typename T, typename ...Args>
    T& Emplace(Layout::Hint h, Args&&... args)
    {
        auto& _t = m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        _t->LayoutHint(h);
        return *dynamic_cast<T*>(_t.get());
    }

    virtual bool Focused() const override { return m_Focused; }
    virtual void Focused(bool v) override
    {
        m_Focused = v;

        // Make sure all sub components are also unfocused.
        if (!v)
        {
            Event _e{ Event::Type::Unfocused };
            for (auto& _c : m_Components)
            {
                if (_c->Focused())
                {
                    _c->Focused(false);
                    _c->AddEvent(_e);
                }
            }
            m_FocusedComponent = nullptr;
        }
    }

    virtual bool Hovering() const override { return m_Hovering; }
    virtual void Hovering(bool v) override
    {
        m_Hovering = v;

        // Make sure all sub components are also unfocused.
        if (!v)
        {
            for (auto& _c : m_Components)
                _c->Hovering(false);
            m_HoveringComponent = nullptr;
        }
    }

    /**
     * Remove a <code>Component</code> from this <code>Container</code> using its index.
     * @param index
     */
    void Remove(int index)
    {
        // Make sure hovering and focussed dont point to nothing
        auto& a = m_Components.at(index);
        if (a == m_HoveringComponent)
            m_HoveringComponent = nullptr;

        if (a == m_FocusedComponent)
            m_FocusedComponent = nullptr;

        m_Components.erase(m_Components.begin() + index);
    }

    /**
     * Remove all components.
     */
    void Clear() { m_Components.clear(); m_HoveringComponent = nullptr, m_FocusedComponent = nullptr; }

    int Cursor() const override { return m_Pressed && m_FocusedComponent ? m_FocusedComponent->Cursor() : m_HoveringComponent ? m_HoveringComponent->Cursor() : m_Cursor; }

    void Render(CommandCollection& d) override
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

    void Update(const Vec4<int>& viewport) override
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
                _c->Update(viewport);
            }

            // If a sub component gained focus on its own, also focus this container.
            if (m_FocusedComponent == nullptr && _c->Focused())
                m_FocusedComponent = _c, Focused(true);
        }

        // If the focused component lost focus, check if it swapped to other component, otherwise
        // remove focus from this container as well.
        if (m_FocusedComponent && !m_FocusedComponent->Focused())
        {
            m_FocusedComponent = nullptr;

            bool _found = false;
            for (auto& _c : m_Components)
                if (_c->Focused())
                    m_FocusedComponent = _c, Focused(true), _found = true;

            if (!_found)
                Focused(false), m_FocusedComponent = nullptr;
        }
    };

    /**
     * Remove a <code>Component</code> from this <code>Container</code> using its iterator.
     * @param iterator
     */
    template<typename T>
    void Erase(T& i)
    {
        if (i->get() == m_HoveringComponent)
            m_HoveringComponent = nullptr;
        if (i->get() == m_FocusedComponent)
            m_FocusedComponent = nullptr;
        m_Components.erase(i, m_Components.end()); 
    }

    /**
     * Get all components.
     * @return <code>ComponentCollection</code>
     */
    Collection& Components() { return m_Components; }

    virtual Type* FocusedComponent() { return m_FocusedComponent; }
    virtual Type* HoveringComponent() { return m_HoveringComponent; }
    virtual void FocusedComponent(Type* a) { m_FocusedComponent = a; }
    virtual void HoveringComponent(Type* a) { m_HoveringComponent = a; }

protected:
    Collection m_Components;
    Type* m_FocusedComponent = 0;
    Type* m_HoveringComponent = 0;
    
    bool m_Pressed = false;

private:
    /**
     * Determines the hovering and focussed and sends 
     * appropriate events to those components
     */
    void Determine(Event& e)
    {
        // Determine the next hovering
        Type* _nextHover = 0;
        for (auto& _c : m_Components)
            if (_c->Visible())
                if (_c->WithinBounds({ e.x, e.y }))
                    _nextHover = _c;

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
};