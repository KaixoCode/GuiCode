#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/Container.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Menu Base -----------------------------------
// --------------------------------------------------------------------------

/**
 * Basis for a <code>Menu</code>
 */
class MenuBase : public Container
{
public:
    MenuBase();

    /**
     * Adjust the default button size, can be overwritten on a per button basis.
     * @param s size, -1 to set no default
     */
    void ButtonSize(const Vec2<int>& s) { m_ButtonSize = s; }

    /**
     * Adjust the default button width, can be overwritten on a per button basis.
     * @param w width, -1 to set no default
     */
    void ButtonWidth(int w) { m_ButtonSize.width = w; }

    /**
     * Adjust the default button height, can be overwritten on a per button basis.
     * @param h height, -1 to set no default
     */
    void ButtonHeight(int h) { m_ButtonSize.height = h; }

    bool WithinBounds(const Vec2<int>& pos) const override;

    void Clear() override
    {
        m_Prev = nullptr;
        Container::Clear();
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
        auto& _ta = *dynamic_cast<T*>(_t.get());
        if constexpr (std::is_base_of_v<ButtonBase, T>)
        {
            if (m_ButtonSize.width != -1)
                _ta.Width(m_ButtonSize.width);
            if (m_ButtonSize.height != -1)
                _ta.Height(m_ButtonSize.height);

            _ta.InMenu(true);

            if (m_Prev)
            {
                auto p = dynamic_cast<ButtonBase*>(m_Prev);
                p->PostButton(&_ta);
                _ta.PreButton(p);
            }
            m_Prev = &_ta;
        }
        return _ta;
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
        
        auto& _ta = *dynamic_cast<T*>(_t.get());
        if constexpr (std::is_base_of_v<ButtonBase, T>)
        {
            if (m_ButtonSize.width != -1)
                _ta.Width(m_ButtonSize.width);
            if (m_ButtonSize.height != -1)
                _ta.Height(m_ButtonSize.height);

            _ta.InMenu(true);

            if (m_Prev)
            {
                auto p = dynamic_cast<ButtonBase*>(m_Prev);
                p->PostButton(&_ta);
                _ta.PreButton(p);
            }
            m_Prev = &_ta;
        }
        return _ta;
    }

protected:
    Vec2<int> m_ButtonSize{ -1, -1 };

private:
    Component* m_Prev = nullptr;
};

// --------------------------------------------------------------------------
// ------------------------------- Menu -------------------------------------
// --------------------------------------------------------------------------

/**
 * Templated <code>Menu</code> class, allows fully customized behaviour and graphics.
 * @tparam MenuGraphics a class with a static Render method that takes in the <code>Menu</code> and the <code>CommandCollection</code>.
 * @tparam MenuType the type of menu, must be a class that inherits MenuBase.
 */
template<typename MenuGraphics, typename MenuType = MenuBase>
class Menu : public MenuType
{
public:
    using MenuType::MenuType;

    void Render(CommandCollection& d) override
    {
        MenuGraphics::Render(*this, d);
        MenuType::Render(d);
    }
};