#pragma once
#include "GuiCode/ui/components/Component.hpp"
#include "GuiCode/ui/components/LayoutManager.hpp"
#include "GuiCode/ui/components/Layout.hpp"

class Container : public Component
{
public:
    Container(Vec2<int> size = { 0, 0 });

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

    /**
     * Remove a <code>Component</code> from this <code>Container</code> using its index.
     * @param index
     */
    void Remove(int index);

    /**
     * Remove all components.
     */
    void Clear() { m_Components.clear(); m_Hovering = nullptr, m_Focussed = nullptr; }

    int Cursor() const override { return m_Pressed && m_Focussed ? m_Focussed->Cursor() : m_Hovering ? m_Hovering->Cursor() : m_Cursor; }

    void Render(CommandCollection& d) override;
    void Update(const Vec4<int>& viewport) override;

    /**
     * Remove a <code>Component</code> from this <code>Container</code> using its iterator.
     * @param iterator
     */
    void Erase(ComponentCollection::const_iterator& i) { m_Components.erase(i, m_Components.end()); m_Hovering = nullptr, m_Focussed = nullptr; }

    /**
     * Get all components.
     * @return <code>ComponentCollection</code>
     */
    ComponentCollection& Components() { return m_Components; }

protected:
    ComponentCollection m_Components;
    Component* m_Focussed = 0;
    Component* m_Hovering = 0;
    
    bool m_Pressed = false;

private:
    /**
     * Determines the hovering and focussed and sends 
     * appropriate events to those components
     */
    void Determine(Event& e);
};