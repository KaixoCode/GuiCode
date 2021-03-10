#include "GuiCode/pch.hpp"
#include "GuiCode/ui/window/SmallFrame.hpp"

void SmallFrame::Update(const Vec4<int>& viewport)
{
    bool _maxi = IsMaximized(GetWin32Handle());
    int _offset = _maxi ? -8 : 0;
    m_Panel->Size({ Width(), Height() - 32 + _offset });
    m_Panel->Position({ 0, 0 });
    m_CloseButton->Position({ Width() - 46 + _offset, Height() - 32 + _offset });
    Window::Update(viewport);
}

void SmallFrame::Render(CommandCollection& d)
{
    using namespace Graphics;
    Window::Render(d);
    bool _maxi = IsMaximized(GetWin32Handle());
    int _offset = _maxi ? -8 : 0;
    int _x = -_offset;
    int _y = Height() - 32 + _offset;
    static const std::string _line = "|";

    d.Command<TexturedQuad>(Graphics::Textures::Logo, _x + 12, Height() - 26 + _offset, 20, 20);
    d.Command<Fill>(::Color{ 179, 179, 179, 255 });
    d.Command<TextAlign>(Align::CENTER, Align::CENTER);
    d.Command<Text>(&_line, _x += 45, _y + 16);
    d.Command<Fill>(::Color{ 179, 179, 179, 255 });
    d.Command<TextAlign>(Align::LEFT, Align::CENTER);
    d.Command<Text>(&m_Name, _x + 9, _y + 16);
}