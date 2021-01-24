#pragma once
#include "GuiCode/ui/components/scrollbars/Scrollbar.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// -------------------------- Scrollbar Type --------------------------------
// --------------------------------------------------------------------------

namespace ScrollbarType
{
    // --------------------------------------------------------------------------
    // ----------------------------- Vertical -----------------------------------
    // --------------------------------------------------------------------------

    class Vertical : public ScrollbarBase
    {
    public:
        Vertical();
    
        virtual bool Hovering() const override { return m_Hovering | m_Dragging; }

    protected:
        bool m_Dragging = false;
    };

    // --------------------------------------------------------------------------
    // ---------------------------- Horizontal ----------------------------------
    // --------------------------------------------------------------------------

    class Horizontal : public ScrollbarBase
    {
    public:
        Horizontal();

        virtual bool Hovering() const override { return m_Hovering | m_Dragging; }

    protected:
        bool m_Dragging = false;
    };
}