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

    /**
     * Vertical scrollbar.
     */
    class Vertical : public ScrollbarBase
    {
    public:
        /**
         * Constructor
         */
        Vertical();
    
        virtual bool Hovering() const override { return m_Hovering | m_Dragging; }
        virtual bool Dragging() const override { return m_Dragging; }

    protected:
        bool m_Dragging = false;
    };

    // --------------------------------------------------------------------------
    // ---------------------------- Horizontal ----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Horizontal scrollbar.
     */
    class Horizontal : public ScrollbarBase
    {
    public:
        /**
         * Constructor
         */
        Horizontal();

        virtual bool Hovering() const override { return m_Hovering | m_Dragging; }
        virtual bool Dragging() const override { return m_Dragging; }

    protected:
        bool m_Dragging = false;
    };
}