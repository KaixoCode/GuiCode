#include "GuiCode/pch.hpp"
#include "GuiCode/ui/window/ComponentFrame.hpp"

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
        //m_Component->Position({ 1, 1 });
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

ComponentFrame::ComponentFrame(const WindowData& d)
    : Window(d),
    m_Panel(&Emplace<::ComponentFramePanel>())
{
    Size({ 100, 100 });

    m_Listener += [this](Event::KeyPressed& e)
    {
        if (e.key == Key::ESC && Visible())
            Close();
    };
}

void ComponentFrame::Update(const Vec4<int>& viewport)
{
    if (m_Component)
    {
        Container* _cont = dynamic_cast<Container*>(m_Component);
        if (_cont)
        {
            int sx = 0, bx = m_Component->Width();
            int sy = m_Component->Y(), by = m_Component->Y() + m_Component->Height();
            std::function<void(::Component*)> _checkSize = [&](::Component* comp)
            {
                Container* _cont = dynamic_cast<Container*>(comp);
                if (_cont)
                    for (auto& _c : _cont->Components())
                    {
                        if (!_c->Visible())
                            continue;
                        int _nx = _c->X() + _c->Width();
                        int _ny = _c->Y();
                        if (_nx > bx)
                            bx = _nx;

                        if (_ny < sy)
                            sy = _ny;

                        _checkSize(_c.get());
                    }
            };

            _checkSize(m_Component);

            int finalx = (int)std::ceil(std::max((bx + 4) / m_Scale, 5.0f));
            int finaly = (int)std::ceil(std::max((by - sy + 5) / m_Scale, 5.0f));

            Size({ finalx, finaly });
            m_Panel->Size({ finalx, finaly });
            m_Component->Position({ 2, (int)std::floor(Height() - m_Component->Height() - 2 / m_Scale) });
        }
        else
        {
            Size(
                (int)std::ceil(std::max((m_Component->Width() + 2) / m_Scale, 5.0f)),
                (int)std::ceil(std::max((m_Component->Height() + 2) / m_Scale, 5.0f))
            );

            m_Panel->Size({ m_Component->Width(), m_Component->Height() });
        }

    }
    m_Panel->Position({ 0, 0 });
    m_Panel->Background(::Color{ 0, 0, 0, 0 });
    Color(::Color{ 0, 0, 0, 0 });
    Window::Update(viewport);
}

void ComponentFrame::Render(CommandCollection& d)
{
    Window::Render(d);
}

void ComponentFrame::Component(::Component* component)
{
    if (m_Component != nullptr)
        m_Component->Focused(false);

    m_Panel->Component(component);
    m_Component = component;
    if (component)
        component->Focused(true);
}