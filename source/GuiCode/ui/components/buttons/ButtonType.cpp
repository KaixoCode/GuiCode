#include "GuiCode/ui/components/buttons/ButtonType.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Button Type ---------------------------------
// --------------------------------------------------------------------------

namespace ButtonType
{
    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------
    
    Normal::Normal(Callback c, const std::string& name, Key key)
        : ButtonBase(name, key), m_Callback(c)
    {
        m_Listener += [this](Event::MousePressed& event)
        {
            if (event.button == Event::MouseButton::LEFT && Hovering())
                m_Active = true;
        };

        m_Listener += [this](Event::MouseReleased& event)
        {
            if (event.button == Event::MouseButton::LEFT)
            {
                if (!Disabled() && Hovering() && m_Active)
                    m_Callback();
                
                m_Active = false;
            }
        };

        m_Listener += [this](Event::KeyPressed& event)
        {
            if (m_KeyCombo == event && !Disabled())
                m_Callback();

            if (event.key == Key::ENTER && Focused() && !Active() && !Disabled())
                m_Active = true, m_Callback();

            if (!Focused())
                return;

            if (m_InMenu && event.key == Key::DOWN && m_Post != nullptr)
            {
                event.key = -1;
                Focused(false);
                m_Post->Focused(true);
            }

            if (m_InMenu && event.key == Key::UP && m_Pre != nullptr)
            {
                event.key = -1;
                Focused(false);
                m_Pre->Focused(true);
            }
        };

        m_Listener += [this](Event::KeyReleased& event)
        {
            if (event.key == Key::ENTER)
                m_Active = false;
        };
    }

    void Normal::Update(const Vec4<int>& v)
    {
        if (!Focused())
            m_Active = false;
        ButtonBase::Update(v);
    }

    // --------------------------------------------------------------------------
    // ------------------------------- Toggle -----------------------------------
    // --------------------------------------------------------------------------

    Toggle::Toggle(bool* a, const std::string& name, Key key)
        : m_Link(a), ButtonBase(name, key)
    {
        SetupCallbacks();
    };

    Toggle::Toggle(ToggleCallback c, const std::string& name, Key key)
        : m_ToggleCallback(c), ButtonBase(name, key)
    {
        SetupCallbacks();
    };

    Toggle::Toggle(const std::string& name, Key key)
        : ButtonBase(name, Key::NONE)
    {
        SetupCallbacks();
    };

    void Toggle::Update(const Vec4<int>& viewport)
    {
        if (m_Link)
            m_Active = *m_Link;
        
        ButtonBase::Update(viewport);
    }

    void Toggle::SetupCallbacks()
    {
        m_Listener += [this](Event::KeyPressed& event)
        {
            if ((m_KeyCombo == event || (event.key == Key::ENTER && Focused() && !m_EnterPressed)) && !Disabled())
            {
                if (event.key == Key::ENTER && Focused()) 
                    m_EnterPressed = true;
                
                if (m_Link)
                {
                    *m_Link ^= true;
                    m_Active = *m_Link;
                }
                else if (m_ToggleCallback)
                {
                    m_Active ^= true;
                    m_ToggleCallback(m_Active);
                }
                else
                    m_Active ^= true;
            }

            if (!Focused())
                return;

            if (m_InMenu && event.key == Key::DOWN && m_Post != nullptr)
            {
                event.key = -1;
                Focused(false);
                m_Post->Focused(true);
            }

            if (m_InMenu && event.key == Key::UP && m_Pre != nullptr)
            {
                event.key = -1;
                Focused(false);
                m_Pre->Focused(true);
            }
        };

        m_Listener += [this](Event::KeyReleased& event)
        {
            if (event.key == Key::ENTER)
                m_EnterPressed = false;
        };

        m_Listener += [this](Event::MousePressed& event)
        {
            if (event.button == Event::MouseButton::LEFT && !Disabled() && Component::WithinBounds({ event.x, event.y }))
            {
                if (m_Link)
                {
                    *m_Link ^= true;
                    m_Active = *m_Link;
                }
                else if (m_ToggleCallback)
                {
                    m_Active ^= true;
                    m_ToggleCallback(m_Active);
                }
                else
                    m_Active ^= true;
            }
        };
    }

    // --------------------------------------------------------------------------
    // ---------------------------- Focus Toggle --------------------------------
    // --------------------------------------------------------------------------

    void FocusToggle::SetupCallbacks()
    {
        m_Listener += [this](Event::Unfocused& event)
        {
            Active(false);
        };

        m_Listener += [this](Event::KeyPressed& event)
        {
            if (event.key == Key::ENTER && Focused())
                event.key = -1;
        };
    }

    void FocusToggle::Update(const Vec4<int>& viewport)
    {
        if (!Focused())
            Active(false);

        if (!::Graphics::WindowFocused())
        {
            Active(false);
        }

        Toggle::Update(viewport);
    }

    // --------------------------------------------------------------------------
    // -------------------------------- Hover -----------------------------------
    // --------------------------------------------------------------------------

    Hover::Hover(const std::string& name)
        : ButtonBase(name, Key::NONE)
    {
        m_Listener += [this](Event::KeyPressed& event)
        {
            if (m_KeyCombo == event && !Disabled())
            {
                m_Active ^= true;
            }
        };

        m_Listener += [this](Event::MouseEntered& event)
        {
            m_Hovering = true;
            m_Active = true;
        };

        m_Listener += [this](Event::MouseExited& event)
        {
            m_Hovering = false;
            m_Active = false;
        };
    };

    void Hover::Update(const Vec4<int>& v)
    {
        ButtonBase::Update(v);
    }
}