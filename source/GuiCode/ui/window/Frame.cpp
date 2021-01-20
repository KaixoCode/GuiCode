#include "GuiCode/pch.hpp"
#include "GuiCode/ui/window/Frame.hpp"

void Frame::Update(const Vec4<int>& viewport)
{
    bool _maxi = IsMaximized(GetWin32Handle());
    int _offset = _maxi ? -8 : 0;
    m_Panel->Size({ Width(), Height() - 32 + _offset });
    m_Panel->Position({ 0, 0 });
    int _x = Width() - 46 + _offset;
    m_CloseButton->Position({ _x, Height() - 32 + _offset });
    if (m_Resizable)
    {
        _x -= 46;
        m_MaxiButton1->Visible(_maxi);
        m_MaxiButton2->Visible(!_maxi);
        m_MaxiButton1->Position({ _x, Height() - 32 + _offset });
        m_MaxiButton2->Position({ _x, Height() - 32 + _offset });
    }
    else
    {
        m_MaxiButton1->Visible(false);
        m_MaxiButton2->Visible(false);
    }
    m_MinimButton->Position({ _x -= 46, Height() - 32 + _offset });
    Window::Update(viewport);
}

void Frame::Render(CommandCollection& d)
{
    using namespace Graphics;
    Window::Render(d);
    bool _maxi = IsMaximized(GetWin32Handle());
    int _offset = _maxi ? -8 : 0;
    int _x = -_offset;
    int _y = Height() - 32 + _offset;
    static const std::string _line = "|";

    d.Command<TexturedQuad>(Graphics::Textures::Logo, _x + 12, Height() - 26 + _offset, 20, 20);
    d.Command<Fill>(Theme::Get(Theme::TITLEBAR_TEXT));
    d.Command<TextAlign>(Align::CENTER, Align::CENTER);
    d.Command<Text>(&_line, _x += 45, _y + 16);

    if (m_Menu->MenuSize() != 0)
    {
        m_Menu->Position({ _x + 6, _y });
        _x += m_Menu->Width() + 6;
        d.Command<TextAlign>(Align::CENTER, Align::CENTER);
        d.Command<Text>(&_line, _x += 6, _y + 16);
    }

    d.Command<Fill>(Theme::Get(Theme::TITLEBAR_TEXT));
    d.Command<TextAlign>(Align::LEFT, Align::CENTER);
    d.Command<Text>(&m_Name, _x + 9, _y + 16.5);
}