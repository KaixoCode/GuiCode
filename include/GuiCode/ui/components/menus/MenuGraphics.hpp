#pragma once
#include "GuiCode/ui/components/menus/Menu.hpp"

// --------------------------------------------------------------------------
// -------------------------- Menu Graphics ---------------------------------
// --------------------------------------------------------------------------

namespace MenuGraphics
{
    // --------------------------------------------------------------------------
    // ----------------------------- Horizontal ---------------------------------
    // --------------------------------------------------------------------------
    
    /**
     * Horizontal menu.
     */
    struct Horizontal 
    { 
        /**
         * Render
         * @param m the <code>Menu</code>
         * @param d the <code>CommandCollection</code>
         */
        static void Render(MenuBase& m, CommandCollection& d); 
    };

    // --------------------------------------------------------------------------
    // ------------------------------ Vertical ----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Vertical menu.
     */
    struct Vertical
    {
        /**
         * Render
         * @param m the <code>Menu</code>
         * @param d the <code>CommandCollection</code>
         */
        static void Render(MenuBase& m, CommandCollection& d);
    };
}
