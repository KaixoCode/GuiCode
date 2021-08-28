#include "GuiCode/ui/components/buttons/ButtonType.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Button Type ---------------------------------
// --------------------------------------------------------------------------

namespace ButtonType
{
    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------
    
    Normal::Normal(ButtonCallback c, const std::string& name, Key key)
        : ButtonBase(name, key), m_Callback(c)
    {
        m_Listener += [this](Event::MousePressed& event)
        {
            m_NeedsRedraw = true;
            if (event.button == Event::MouseButton::LEFT && Hovering())
                Active(true);
        };

        m_Listener += [this](Event::MouseExited& event)
        {
            m_NeedsRedraw = true;
        };

        m_Listener += [this](Event::MouseEntered& event)
        {
            m_NeedsRedraw = true;
        };

        m_Listener += [this](Event::MouseReleased& event)
        {
            if (event.button == Event::MouseButton::LEFT)
            {
                m_NeedsRedraw = true;
                if (!Disabled() && Hovering())
                {
                    m_Callback();
                }
                Active(false);
            }
        };

        m_Listener += [this](Event::KeyPressed& event)
        {
            if (m_KeyCombo == event && !Disabled())
                m_Callback();

            if (event.key == Key::ENTER && Focused() && !Active() && !Disabled())
                Active(true), m_Callback();

            if (!Focused())
                return;

            if (m_InMenu && event.key == Key::DOWN && m_Post != nullptr)
            {
                event.key = -1;
                ButtonBase* post = m_Post;
                while (post && post->Disabled())
                {
                    post = post->PostButton();
                }

                if (post)
                {
                    Focused(false);
                    post->Focused(true);
                }
            }

            if (m_InMenu && event.key == Key::UP && m_Pre != nullptr)
            {
                event.key = -1;
                ButtonBase* pre = m_Pre;
                while (pre && pre->Disabled())
                {
                    pre = pre->PreButton();
                }

                if (pre)
                {
                    Focused(false);
                    pre->Focused(true);
                }
            }
        };

        m_Listener += [this](Event::KeyReleased& event)
        {
            if (event.key == Key::ENTER)
                Active(false);
        };

        m_Listener += [this](Event::Unfocused& event)
        {
            Active(false);
        };
    }

    void Normal::Update(const Vec4<int>& v)
    {
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
        {
            if (*m_Link != m_Active)
                m_NeedsRedraw = true;

            Active(*m_Link);;
        }
        ButtonBase::Update(viewport);
    }

    void Toggle::SetupCallbacks()
    {
        m_Listener += [this](Event::MouseExited& event)
        {
            m_NeedsRedraw = true;
            m_Hovering = false;
        };

        m_Listener += [this](Event::MouseEntered& event)
        {
            m_NeedsRedraw = true;
            m_Hovering = true;
        };

        m_Listener += [this](Event::KeyPressed& event)
        {
            if ((m_KeyCombo == event || (event.key == Key::ENTER && Focused() && !m_EnterPressed)) && !Disabled())
            {
                if (event.key == Key::ENTER && Focused()) 
                    m_EnterPressed = true;
                
                if (m_Link)
                {
                    *m_Link ^= true;
                    Active(*m_Link);
                }
                else if (m_ToggleCallback)
                {
                    Active(!Active());
                    m_ToggleCallback(m_Active);
                }
                else
                    Active(!Active());
            }

            if (!Focused())
                return;

            if (m_InMenu && event.key == Key::DOWN && m_Post != nullptr)
            {
                event.key = -1;
                ButtonBase* post = m_Post;
                while (post && post->Disabled())
                {
                    post = post->PostButton();
                }

                if (post)
                {
                    Focused(false);
                    post->Focused(true);
                }
            }

            if (m_InMenu && event.key == Key::UP && m_Pre != nullptr)
            {
                event.key = -1;
                ButtonBase* pre = m_Pre;
                while (pre && pre->Disabled())
                {
                    pre = pre->PreButton();
                }

                if (pre)
                {
                    Focused(false);
                    pre->Focused(true);
                }
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
                m_NeedsRedraw = true;
                if (m_Link)
                {
                    *m_Link ^= true;
                    Active(*m_Link);
                }
                else if (m_ToggleCallback)
                {
                    Active(!Active());
                    m_ToggleCallback(m_Active);
                }
                else
                    Active(!Active());
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
            m_NeedsRedraw = true;
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
            m_NeedsRedraw = true;
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
                Active(!Active());
            }
        };

        m_Listener += [this](Event::MouseEntered& event)
        {
            m_NeedsRedraw = true;
            m_Hovering = true;
            Active(true);
        };

        m_Listener += [this](Event::MouseExited& event)
        {
            m_NeedsRedraw = true;
            m_Hovering = false;
            Active(false);
        };
    };

    void Hover::Update(const Vec4<int>& v)
    {
        ButtonBase::Update(v);
    }
}