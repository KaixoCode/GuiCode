#pragma once
#include "GuiCode/ui/components/Component.hpp"
#include "GuiCode/ui/components/LayoutManager.hpp"
#include "GuiCode/ui/components/Layout.hpp"
class ButtonBase;
class Container : public Component
{
public:
    Container();

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
        return *dynamic_cast<T*>(_t.get());
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

    virtual void Focused(bool v) override;
    virtual bool Focused() const override { return m_Focused; }
    virtual void Hovering(bool v) override;
    virtual bool Hovering() const override { return m_Hovering; }

    /**
     * Remove a <code>Component</code> from this <code>Container</code> using its index.
     * @param index
     */
    void Remove(int index);

    /**
     * Remove all components.
     */
    virtual void Clear() { m_Components.clear(); m_HoveringComponent = nullptr, m_FocusedComponent = nullptr; }

    int Cursor() const override { return m_Pressed && m_FocusedComponent ? m_FocusedComponent->Cursor() : m_HoveringComponent ? m_HoveringComponent->Cursor() : m_Cursor; }

    void Render(CommandCollection& d) override;
    void Update(const Vec4<int>& viewport) override;

    /**
     * Remove a <code>Component</code> from this <code>Container</code> using its iterator.
     * @param iterator
     */
    void Erase(ComponentCollection::const_iterator& i) 
    {
        m_HoveringComponent = nullptr;
        m_FocusedComponent = nullptr;
        m_Components.erase(i, m_Components.end()); 
    }

    /**
     * Remove a <code>Component</code> from this <code>Container</code> using a reference.
     * @param component
     */
    void Erase(Component& i)
    {
        m_HoveringComponent = nullptr;
        m_FocusedComponent = nullptr;
        m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(), [&](auto& c) { return c.get() == &i; }));
    }

    /**
     * Remove a <code>Component</code> from this <code>Container</code> using a reference.
     * @param component
     */
    void EraseLater(Component& i)
    {
        m_HoveringComponent = nullptr;
        m_FocusedComponent = nullptr;
        m_EraseLater.push_back(&i);
    }

    /**
     * Get all components.
     * @return <code>ComponentCollection</code>
     */
    ComponentCollection& Components() { return m_Components; }

    virtual Component* FocusedComponent() { return m_FocusedComponent; }
    virtual Component* HoveringComponent() { return m_HoveringComponent; }
    virtual void FocusedComponent(Component* a) { m_FocusedComponent = a; }
    virtual void HoveringComponent(Component* a) { m_HoveringComponent = a; }

protected:
    ComponentCollection m_Components;
    Component* m_FocusedComponent = 0;
    Component* m_HoveringComponent = 0;
    
    bool m_Pressed = false;

private:
    /**
     * Determines the hovering and focussed and sends 
     * appropriate events to those components
     */
    void Determine(Event& e);

    // Used to delete a component at a time where it won't crash.
    std::vector<Component*> m_EraseLater;
};