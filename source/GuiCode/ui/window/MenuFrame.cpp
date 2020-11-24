#include "GuiCode/pch.hpp"
#include "GuiCode/ui/window/MenuFrame.hpp"

void MenuFrame::Update(const Vec4<int>& viewport)
{
    bool _maxi = IsMaximized(GetWin32Handle());
    int _offset = _maxi ? -8 : 0;
    m_Panel->Size({ Width(), Height() });
    m_Panel->Position({ 0, 0 });
    m_Panel->Background(Color{ 255, 0, 0, 255 });
    Window::Update(viewport);
}

void MenuFrame::Render(CommandCollection& d)
{
    Window::Render(d);
}