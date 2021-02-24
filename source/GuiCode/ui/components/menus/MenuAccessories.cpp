#include "GuiCode/ui/components/menus/MenuAccessories.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu accessories -------------------------------
// --------------------------------------------------------------------------

namespace MenuAccessories
{
    using namespace Graphics;
    
    // --------------------------------------------------------------------------
    // ------------------------------ Divider -----------------------------------
    // --------------------------------------------------------------------------
    
    void Divider::Render(CommandCollection& d)
    {
        
        d.Command<Fill>(m_Color);
        d.Command<Quad>(X() + m_PaddingX, Y() + m_PaddingY, Width() - m_PaddingX * 2, m_Stroke);
    }

    void VerticalDivider::Render(CommandCollection& d)
    {
        d.Command<Fill>(m_Color);
        d.Command<Quad>(X() + m_PaddingX, Y() + m_PaddingY, m_Stroke, Height() - m_PaddingY * 2);
    }
}