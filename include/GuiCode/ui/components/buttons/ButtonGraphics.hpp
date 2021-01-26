#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/buttons/Button.hpp"
#include "GuiCode/ui/components/buttons/ButtonType.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"
#include "GuiCode/ui/graphics/Theme.hpp"

// --------------------------------------------------------------------------
// -------------------------- Button Graphics -------------------------------
// --------------------------------------------------------------------------

namespace ButtonGraphics
{
    using namespace Graphics;

    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------

    template<Align Hori = Align::CENTER, Align Vert = Align::CENTER>
    class Normal
    {
        static void Render(ButtonBase& b, CommandCollection& d)
        {
            // TODO    
        }
    };

    // --------------------------------------------------------------------------
    // -------------------------------- Menu ------------------------------------
    // --------------------------------------------------------------------------

    struct Menu
    {
        template<typename Type = ButtonBase>
        static void Render(Type& b, CommandCollection& d)
        {
            int _padding = 20;
            Color _c1;
            Color _c2 = Theme::Get((b.Disabled() ? Theme::MENU_DISABLED_BUTTON_TEXT : Theme::MENU_BUTTON_TEXT)) * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            Color _c3 = Theme::Get((b.Disabled() ? Theme::MENU_DISABLED_BUTTON_DARKER_TEXT : Theme::MENU_BUTTON_DARKER_TEXT)) * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            
            _c1 = Theme::Get(b.Disabled() ?
                b.Active() ? Theme::MENU_DISABLED_BUTTON_BACKGROUND_PRESS : b.Hovering() ? Theme::MENU_DISABLED_BUTTON_BACKGROUND_HOVER : Theme::MENU_DISABLED_BUTTON_BACKGROUND
                :
                b.Active() ? Theme::MENU_BUTTON_BACKGROUND_PRESS : b.Hovering() ? Theme::MENU_BUTTON_BACKGROUND_HOVER : Theme::MENU_BUTTON_BACKGROUND);
            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());
            d.Command<Font>(Fonts::Gidole14, 14.0f);
            d.Command<Fill>(_c2);
            d.Command<TextAlign>(Align::LEFT, Align::CENTER);
            d.Command<Text>(&b.Name(), _padding + b.X() + 6, b.Y() + b.Height() / 2);
            d.Command<Fill>(_c3);
            d.Command<TextAlign>(Align::RIGHT, Align::CENTER);
            if (b.KeyCombo() != Key::NONE)
                d.Command<Text>(&b.KeyCombo().ToString(), b.X() + b.Width() - 6, b.Y() + b.Height() / 2);
        };

        template<>
        static void Render<Button<Menu, ButtonType::List>>(Button<Menu, ButtonType::List>& b, CommandCollection& d)
        {
            int _padding = 20;
            Color _c1;
            Color _c2 = Theme::Get((b.Disabled() ? Theme::MENU_DISABLED_BUTTON_TEXT : Theme::MENU_BUTTON_TEXT)) * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            Color _c3 = Theme::Get((b.Disabled() ? Theme::MENU_DISABLED_BUTTON_DARKER_TEXT : Theme::MENU_BUTTON_DARKER_TEXT)) * (Graphics::WindowFocused() ? 1.0f : 0.8f);

            _c1 = Theme::Get(b.Disabled() ?
                b.Hovering() ? Theme::MENU_DISABLED_BUTTON_BACKGROUND_HOVER : Theme::MENU_DISABLED_BUTTON_BACKGROUND
                :
                b.Hovering() ? Theme::MENU_BUTTON_BACKGROUND_HOVER : Theme::MENU_BUTTON_BACKGROUND);

            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());

            if (b.Selected())
            {
                d.Command<Fill>(Theme::Get(Theme::MENU_BUTTON_BACKGROUND_PRESS));
                int _p = 2;
                d.Command<Quad>(b.Position() + Vec2<int>{ _p, _p }, Vec2<int>{ b.Size().y - _p * 2, b.Size().y - _p * 2 });
            }
            d.Command<Font>(Fonts::Gidole14, 14.0f);
            d.Command<Fill>(_c2);
            d.Command<TextAlign>(Align::LEFT, Align::CENTER);
            d.Command<Text>(&b.Name(), _padding + b.X() + 6, b.Y() + b.Height() / 2);
            d.Command<Fill>(_c3);
            d.Command<TextAlign>(Align::RIGHT, Align::CENTER);
            if (b.KeyCombo() != Key::NONE)
                d.Command<Text>(&b.KeyCombo().ToString(), b.X() + b.Width() - 6, b.Y() + b.Height() / 2);
        };

        template<>
        static void Render<Button<Menu, ButtonType::Toggle>>(Button<Menu, ButtonType::Toggle>& b, CommandCollection& d)
        {
            int _padding = 20;
            Color _c1;
            Color _c2 = Theme::Get((b.Disabled() ? Theme::MENU_DISABLED_BUTTON_TEXT : Theme::MENU_BUTTON_TEXT)) * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            Color _c3 = Theme::Get((b.Disabled() ? Theme::MENU_DISABLED_BUTTON_DARKER_TEXT : Theme::MENU_BUTTON_DARKER_TEXT)) * (Graphics::WindowFocused() ? 1.0f : 0.8f);
        
            _c1 = Theme::Get(b.Disabled() ?
                b.Hovering() ? Theme::MENU_DISABLED_BUTTON_BACKGROUND_HOVER : Theme::MENU_DISABLED_BUTTON_BACKGROUND
                :
                b.Hovering() ? Theme::MENU_BUTTON_BACKGROUND_HOVER : Theme::MENU_BUTTON_BACKGROUND);

            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());

            if (b.Active())
            {
                d.Command<Fill>(Theme::Get(Theme::MENU_BUTTON_BACKGROUND_PRESS));
                int _p = 2;
                d.Command<Quad>(b.Position() + Vec2<int>{ _p, _p }, Vec2<int>{ b.Size().y - _p * 2, b.Size().y - _p * 2 });
            }
            d.Command<Font>(Fonts::Gidole14, 14.0f);
            d.Command<Fill>(_c2);
            d.Command<TextAlign>(Align::LEFT, Align::CENTER);
            d.Command<Text>(&b.Name(), _padding + b.X() + 6, b.Y() + b.Height() / 2);
            d.Command<Fill>(_c3);
            d.Command<TextAlign>(Align::RIGHT, Align::CENTER);
            if (b.KeyCombo() != Key::NONE)
                d.Command<Text>(&b.KeyCombo().ToString(), b.X() + b.Width() - 6, b.Y() + b.Height() / 2);
        };

        template<>
        static void Render<Button<Menu, ButtonType::Hover>>(Button<Menu, ButtonType::Hover>& b, CommandCollection& d)
        {
            int _padding = 20;
            Color _c1;
            Color _c2 = Theme::Get((b.Disabled() ? Theme::MENU_DISABLED_BUTTON_TEXT : Theme::MENU_BUTTON_TEXT)) * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            Color _c3 = Theme::Get((b.Disabled() ? Theme::MENU_DISABLED_BUTTON_DARKER_TEXT : Theme::MENU_BUTTON_DARKER_TEXT)) * (Graphics::WindowFocused() ? 1.0f : 0.8f);
            
            _c1 = Theme::Get(b.Disabled() ?
                b.Hovering() ? Theme::MENU_DISABLED_BUTTON_BACKGROUND_HOVER : Theme::MENU_DISABLED_BUTTON_BACKGROUND
                :
                b.Hovering() ? Theme::MENU_BUTTON_BACKGROUND_HOVER : Theme::MENU_BUTTON_BACKGROUND);
            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());
            d.Command<Font>(Fonts::Gidole14, 14);
            d.Command<Fill>(_c2);
            d.Command<TextAlign>(Align::LEFT, Align::CENTER);
            d.Command<Text>(&b.Name(), _padding + b.X() + 6, b.Y() + b.Height() / 2);
            d.Command<Fill>(_c3);
            d.Command<TextAlign>(Align::RIGHT, Align::CENTER);
            if (b.KeyCombo() != Key::NONE)
                d.Command<Text>(&b.KeyCombo().ToString(), b.X() + b.Width() - 6, b.Y() + b.Height() / 2);
        };
    };

    // --------------------------------------------------------------------------
    // ------------------------------ Sub Menu ----------------------------------
    // --------------------------------------------------------------------------

    struct SubMenu
    {
        template<typename Type = ButtonBase>
        static void Render(Type& b, CommandCollection& d)
        {
            Menu::Render(b, d);
            int _w = 8;
            int _padding = 4;
            d.Command<Triangle>(b.X() + b.Width() - _w / 2 - _padding, b.Y() + b.Height() / 2, _w / 2, _w, 0.0f);
        }
    };

    // --------------------------------------------------------------------------
    // ----------------------------- Title Menu ---------------------------------
    // --------------------------------------------------------------------------

    struct TitleMenu
    {
        static void Render(ButtonBase& b, CommandCollection& d);
    };

    // --------------------------------------------------------------------------
    // ------------------------------ Textured ----------------------------------
    // --------------------------------------------------------------------------

    template<Align Hori = Align::CENTER, Align Vert = Align::CENTER>
    struct Textured
    {
        template<typename Type = ButtonBase>
        static void Render(ButtonType::Textured<Type>& b, CommandCollection& d)
        {
            Graphics::Texture& _t = b.Active() ? b.m_Texture3 : b.Hovering() ? b.m_Texture2 : b.m_Texture1;
            b.Size({ _t.Width, _t.Height });
            d.Command<TexturedQuad>(_t, b.X(), b.Y(), b.Width(), b.Height());
            d.Command<Fill>(Color{ 1.0f, 1.0f, 1.0f, 1.0f });
            d.Command<Font>(Fonts::Gidole14, 14.0f);
            d.Command<TextAlign>(Hori, Vert);
            if (Hori == Align::CENTER)
                d.Command<Text>(&b.Name(), b.X() + b.Width() / 2, b.Y() + b.Height() / 2);
            else if (Hori == Align::RIGHT)
                d.Command<Text>(&b.Name(), b.X() + b.Width() - 6, b.Y() + b.Height() / 2);
            else if (Hori == Align::LEFT)
                d.Command<Text>(&b.Name(), b.X() + 6, b.Y() + b.Height() / 2);
        }
    };
}
