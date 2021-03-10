#include "GuiCode/ui/components/buttons/ButtonGraphics.hpp"

// --------------------------------------------------------------------------
// -------------------------- Button Graphics -------------------------------
// --------------------------------------------------------------------------

namespace ButtonGraphics
{
    using namespace Graphics;
    
    // --------------------------------------------------------------------------
    // ----------------------------- Title Menu ---------------------------------
    // --------------------------------------------------------------------------

    void TitleMenu::Render(ButtonBase& b, CommandCollection& d)
    {
        Color _c1 = b.Active() ? Color{ 18, 18, 18, 255 } : b.Hovering() ? Color{ 255, 255, 255, 13 } : Color{ 0, 0, 0, 0 };
        Color _c2 = Color{ 255, 255, 255, 255 };
        
        int _padding = 20;
        if (b.Active())
        {
            d.Command<Fill>(Color{ 64, 64, 64, 255 });
            d.Command<Quad>(b.X() - 1, b.Y(), b.Width() + 2, b.Height());
        }
        d.Command<Fill>(_c1);
        d.Command<Quad>(b.X(), b.Y(), b.Width(), b.Height() - 1);
        d.Command<Fill>(_c2);
        d.Command<Font>(Gidole14, 14.0f);
        d.Command<TextAlign>(Align::CENTER, Align::CENTER);
        d.Command<Text>(&b.Name(), Vec2<int>{b.X() + b.Width() / 2, b.Y() + b.Height() / 2});
    }
}
