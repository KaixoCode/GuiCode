#include "GuiCode/ui/components/Component.hpp"

Component::Component()
{
    m_Listener += [this](Event::KeyTyped& e)
    {
        if (!Focused())
            return;

        if (m_TabSwitch)
            return;

        if (e == Key::SHIFT_TAB)
        {
            if (m_BackTabComponent)
            {
                m_BackTabComponent->Focused(true), Focused(false), m_BackTabComponent->m_TabSwitch = true;
                return;
            }
        }
        else if (e.key == Key::TAB)
            if (m_TabComponent)
            {
                m_TabComponent->Focused(true), Focused(false), m_TabComponent->m_TabSwitch = true;
                return;
            }
    };
}

bool Component::WithinBounds(const Vec2<int>& pos) const
{
    return pos.x >= Position().x && pos.x <= Position().x + Size().width
        && pos.y >= Position().y && pos.y <= Position().y + Size().height;
};

void Component::Render(CommandCollection& d)
{
    if (Focused())
    {
        d.Command<Graphics::Fill>(Color{ 255, 0, 0, 10 });
        d.Command<Graphics::_DebugOverlay>(Vec4<int>{ Position(), Size() });
    }
    if (Hovering())
    {
        d.Command<Graphics::Fill>(Color{ 0, 255, 0, 10 });
        d.Command<Graphics::_DebugOverlay>(Vec4<int>{ Position(), Size() });
    }
    m_NeedsRedraw = false;
}

void Component::Update(const Vec4<int>& viewport) 
{
    m_TabSwitch = false;
    m_Viewport = viewport;
}

void Component::TabComponent(Component* c)
{
    m_TabComponent = c;
    if (c && &c->BackTabComponent() == nullptr)
        c->BackTabComponent(this);
}

void Component::BackTabComponent(Component* c)
{
    m_BackTabComponent = c;
    if (c && &c->TabComponent() == nullptr)
        c->TabComponent(this);
}
