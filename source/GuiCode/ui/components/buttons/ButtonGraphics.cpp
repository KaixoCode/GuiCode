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
        Color _c1 = Theme::Get(b.Active() ? Theme::MENU_BACKGROUND : b.Hovering() ? Theme::MENU_BUTTON_BACKGROUND_HOVER : Theme::MENU_BUTTON_BACKGROUND);
        Color _c2 = Theme::Get(Theme::MENU_BUTTON_TEXT);
        int _padding = 20;
        if (b.Active())
        {
            d.Command<Fill>(Theme::Get(Theme::MENU_BORDER));
            d.Command<Quad>(b.X() - 1, b.Y(), b.Width() + 2, b.Height());
        }
        d.Command<Fill>(_c1);
        d.Command<Quad>(b.X(), b.Y(), b.Width(), b.Height() - 1);
        d.Command<Fill>(_c2);
        d.Command<Font>(Fonts::Gidole14, 14.0f);
        d.Command<TextAlign>(Align::CENTER, Align::CENTER);
        d.Command<Text>(&b.Name(), Vec2<int>{b.X() + b.Width() / 2, b.Y() + b.Height() / 2});
    }
}
