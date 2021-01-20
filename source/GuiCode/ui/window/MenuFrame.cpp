#include "GuiCode/pch.hpp"
#include "GuiCode/ui/window/MenuFrame.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu Frame Panel -------------------------------
// --------------------------------------------------------------------------

ComponentFramePanel::ComponentFramePanel()
{
    m_Listener += [this](Event& event)
    {
        if (m_Component)
            m_Component->AddEvent(event);
    };
}

void ComponentFramePanel::Update(const Vec4<int>& viewport)
{
    Panel::Update(viewport);
    if (m_Component)
    {
        m_Component->Position({ 0, 0 });
        m_Component->Update(viewport);
    }
}

void ComponentFramePanel::Render(CommandCollection& d)
{
    Panel::Render(d);
    if (m_Component)
        m_Component->Render(d);
}

// --------------------------------------------------------------------------
// ----------------------------- Menu Frame ---------------------------------
// --------------------------------------------------------------------------

void ComponentFrame::Update(const Vec4<int>& viewport)
{
    if (m_Component)
        Size(m_Component->Width(), m_Component->Height());

    m_Panel->Size({ Width(), Height() });
    m_Panel->Position({ 0, 0 });
    m_Panel->Background(Color{ 255, 0, 0, 255 });
    Window::Update(viewport);
}

void ComponentFrame::Render(CommandCollection& d)
{
    Window::Render(d);
}

void ComponentFrame::Component(::Component* component)
{
    m_Panel->Component(component);
    m_Component = component;
}