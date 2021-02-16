#pragma once
#include "GuiCode/ui/components/menus/Menu.hpp"
#include "GuiCode/ui/components/buttons/Button.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Menu Type -----------------------------------
// --------------------------------------------------------------------------

namespace MenuType
{
    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Normal menu behaviour.
     */
    class Normal : public MenuBase
    {
    public:
        using MenuBase::MenuBase;
    };

    // --------------------------------------------------------------------------
    // ---------------------------- Toggle Hover --------------------------------
    // --------------------------------------------------------------------------

    /**
     * When a single button has been toggled active, when hovering over another button 
     * the active state will switch to that other Button.
     */
    class ToggleHover : public MenuBase
    {
    public:
        using MenuBase::MenuBase;

        void Update(const Vec4<int>& viewport) override;
    };
}