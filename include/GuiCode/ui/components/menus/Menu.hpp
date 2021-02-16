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
    MenuBase()
        : Container()
    {};

    bool WithinBounds(const Vec2<int>& pos) const override;
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