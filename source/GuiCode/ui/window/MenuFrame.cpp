#include "GuiCode/pch.hpp"
#include "GuiCode/ui/window/MenuFrame.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu Frame Panel -------------------------------
// --------------------------------------------------------------------------

MenuFramePanel::MenuFramePanel()
{
    m_Listener += [this](Event& event)
    {
        if (m_Menu)
            m_Menu->AddEvent(event);
    };
}

void MenuFramePanel::Update(const Vec4<int>& viewport)
{
    Panel::Update(viewport);
    if (m_Menu)
        m_Menu->Update(viewport);
}

void MenuFramePanel::Render(CommandCollection& d)
{
    Panel::Render(d);
    if (m_Menu)
        m_Menu->Render(d);
}

// --------------------------------------------------------------------------
// ----------------------------- Menu Frame ---------------------------------
// --------------------------------------------------------------------------

void MenuFrame::Update(const Vec4<int>& viewport)
{
    if (m_Menu)
        Size(m_Menu->Width(), m_Menu->Height());

    m_Panel->Size({ Width(), Height() });
    m_Panel->Position({ 0, 0 });
    m_Panel->Background(Color{ 255, 0, 0, 255 });
    Window::Update(viewport);
}

void MenuFrame::Render(CommandCollection& d)
{
    Window::Render(d);
}

void MenuFrame::SetMenu(MenuBase* menu)
{
    m_Panel->SetMenu(menu);
    m_Menu = menu;
}