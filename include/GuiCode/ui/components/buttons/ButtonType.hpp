#pragma once
#include "GuiCode/ui/components/buttons/Button.hpp"
#include "GuiCode/ui/components/menus/Menu.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Button Type ---------------------------------
// --------------------------------------------------------------------------

namespace ButtonType
{
    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Normal Button behaviour: press to run the Callback.
     */
    class Normal : public ButtonBase
    {
    public:
        /**
         * Constructor
         * @param c callback
         * @param name name
         * @param size size
         * @param key keycombo
         */
        Normal(Callback c = []() {}, const std::string& name = "Button", Key key = Key::NONE);

    private:
        Callback m_Callback;
    };

    // --------------------------------------------------------------------------
    // -------------------------------- List ------------------------------------
    // --------------------------------------------------------------------------

    /**
     * Only a single button all buttons of this type with the same <code>id</code> can be selected at a time. 
     * Works like a radio input. 
     */
    class List : public Normal
    {
    public:
        /**
         * Constructor
         * @param c callback
         * @param name name
         * @param id list id
         * @param size size
         * @param key keycombo
         */
        List(Callback c = []() {}, const std::string& name = "Button",
            int id = 0, Key key = Key::NONE)
            : Normal(c, name, key), m_Id(id)
        {
            m_Listener += [this](Event::MouseReleased& event)
            {
                if (event.button == Event::MouseButton::LEFT && Active())
                {
                    if (!Disabled() && Component::WithinBounds({ event.x, event.y }))
                    {
                        for (auto& _l : m_Lists[m_Id])
                            _l->Selected(false);
                        
                        Selected(true);
                    }
                }
            };

            m_Listener += [this](Event::KeyPressed& event)
            {
                if (event.key == Key::ENTER && Focused() && !Disabled())
                {
                    for (auto& _l : m_Lists[m_Id])
                        _l->Selected(false);

                    Selected(true);
                }
            };

            auto& _exists = m_Lists.find(id);
            if (_exists == m_Lists.end())
                m_Lists.emplace(id, std::vector<List*>());
   
            m_Lists[id].push_back(this);
        };

        /**
         * Select or unselect this <code>Button</code>.
         * @param b select
         */
        void Selected(bool b) 
        { 
            if (b) 
                for (auto& _l : m_Lists[m_Id])
                    _l->Selected(false);
            m_Selected = b; 
        }

        /**
         * Returns true when this <code>Button</code> is selected.
         * @returns true when selected
         */
        bool Selected() { return m_Selected; }

        /**
         * Generates a new unique key to be used for a new List group.
         * @return unique key
         */
        static inline int NewKey()
        {
            m_CurrentKey++;
            return m_CurrentKey;
        }

    protected:
        static inline int m_CurrentKey = 0;
        static inline std::unordered_map<int, std::vector<List*>> m_Lists;
        bool m_Selected = false;
        int m_Id = 0;
        Callback m_Callback;
    };

    // --------------------------------------------------------------------------
    // ------------------------------- Toggle -----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Toggle Button behaviour, click to toggle the state.
     */
    class Toggle : public ButtonBase
    {
    public:
        /**
         * Constructor
         * @param a pointer to a bool, this button will actively read for a change in value, and when clicked change the state of this bool.
         * @param name name
         * @param size size
         * @param key keycombo
         */
        Toggle(bool* a, const std::string& name = "Button",
            Key key = Key::NONE);

        /**
         * Constructor
         * @param a pointer to a bool, this button will actively read for a change in value, and when clicked change the state of this bool.
         * @param name name
         * @param size size
         * @param key keycombo
         */
        Toggle(ToggleCallback a = [] (bool& state) {}, const std::string& name = "Button",
            Key key = Key::NONE);

        /**
         * Constructor
         * @param name name
         * @param size size
         * @param key keycombo
         */
        Toggle(const std::string& name = "Button",
            Key key = Key::NONE);

        auto Active(bool a) -> void override { if (m_Link) *m_Link = a; m_Active = a; if (m_ToggleCallback) m_ToggleCallback(m_Active); }
        bool Active() const override { return m_Active; }

        void Update(const Vec4<int>& viewport) override;

    protected:
        bool* m_Link = { 0 };
        bool m_EnterPressed = false;

        virtual void SetupCallbacks();
        ToggleCallback m_ToggleCallback = nullptr;
    };

    // --------------------------------------------------------------------------
    // ---------------------------- Focus Toggle --------------------------------
    // --------------------------------------------------------------------------

    /**
     * Works the same as Toggle, but becomes inactive when the button loses focus (i.e. mouse is clicked somewhere else)
     */
    class FocusToggle : public Toggle
    {
    public:
        /**
         * See Constructor of <code>ButtonType::Toggle</code>
         */
        template<typename ...Args>
        FocusToggle(Args&& ...a)
            : Toggle(std::forward<Args>(a)...)
        {
            SetupCallbacks();
        }

        void Update(const Vec4<int>& viewport) override;

    protected:
        void SetupCallbacks() override;
        int m_Counter = 0;
    };

    // --------------------------------------------------------------------------
    // -------------------------------- Hover -----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Becomes active when you hover over the button.
     */
    class Hover : public ButtonBase
    {
    public:
        /**
         * Constructor
         * @param name name
         * @param size size
         */
        Hover(const std::string& name = "Button");

        void Update(const Vec4<int>& v) override;
    };

    // --------------------------------------------------------------------------
    // ------------------------------- Textured ---------------------------------
    // --------------------------------------------------------------------------

    /**
     * Button that can take on any other ButtonType, but stores 3 textures for the ButtonGraphics::Textured graphics.
     * @tparam ButtonType button behaviour
     */
    template<typename ButtonType = ButtonBase>
    class Textured : public ButtonType
    {
    public:
        Graphics::Texture& m_Texture1;
        Graphics::Texture& m_Texture2;
        Graphics::Texture& m_Texture3;

        /**
         * Constructor
         * @param t1 texture for the idle state
         * @param t2 texture for the hovering state
         * @param t3 texture for the pressed state
         * @param a arguments for the constructor of template argument <code>ButtonType</code>
         */
        template<typename ...Args>
        Textured(Graphics::Texture& t1, Graphics::Texture& t2, Graphics::Texture& t3, Args&& ...a)
            : m_Texture1{ t1 }, m_Texture2{ t2 }, m_Texture3{ t3 }, ButtonType(std::forward<Args>(a)...)
        {}
    };

    // --------------------------------------------------------------------------
    // --------------------------------- Menu -----------------------------------
    // --------------------------------------------------------------------------
     
    /**
     * Button that also stores a <code>Menu</code>, has template arguments for ButtonGraphics, ButtonType and MenuType
     * @tparam Graphics button graphics
     * @tparam MenuType type of <code>Menu</code>
     * @tparam ButtonType button behaviour
     * @tparam A alignment of the menu, either Align::BOTTOM, Align::RIGHT or Align::LEFT
     */
    template<typename Graphics, typename MenuType = MenuBase, typename ButtonType = Toggle, Align A = Align::BOTTOM>
    class Menu : public ButtonType
    {
    public:

        /** 
         * See constructor of template argument <code>ButtonType</code>.
         */
        template<typename ...Args>
        Menu(Args&&...args)
            : ButtonType(args...), m_Menu(&ButtonType::Emplace<::Menu<Graphics, MenuType>>())
        {
            m_Listener += [this](Event::KeyPressed& event)
            {
                if (!Focused() || Disabled())
                    return;

                if (!Active() && m_InMenu && event.key == Key::DOWN && m_Post != nullptr)
                {
                    event.key = -1;
                    Focused(false);
                    m_Post->Focused(true);
                }

                if (!Active() && m_InMenu && event.key == Key::UP && m_Pre != nullptr)
                {
                    event.key = -1;
                    Focused(false);
                    m_Pre->Focused(true);
                }

                if (Active() && m_InMenu && event.key == Key::LEFT)
                {
                    auto bttn = dynamic_cast<ButtonBase*>(m_Menu->FocusedComponent());
                    if (bttn && bttn->Active())
                        return;

                    m_Menu->FocusedComponent(nullptr);
                    for (auto& i : m_Menu->Components())
                        i->Focused(false);
                    event.key = -1;
                    m_CloseMenu = true;
                }

                if (Active() && m_InMenu && event.key == Key::ESC)
                {
                    auto bttn = dynamic_cast<ButtonBase*>(m_Menu->FocusedComponent());
                    if (bttn && bttn->Active())
                        return;

                    m_Menu->FocusedComponent(nullptr);
                    for (auto& i : m_Menu->Components())
                        i->Focused(false);
                    event.key = -1;
                    m_CloseMenu = true;
                }

                if (!Active() && m_InMenu && event.key == Key::RIGHT && Focused() && Visible())
                {
                    Active(true);
                }
            };
        }

        void Update(const Vec4<int>& viewport) override
        {
            if (m_CloseMenu)
            {
                m_CloseMenu = false;
                Active(false);
            }

            if (!m_Menu->Visible() && Active() && Focused())
            {
                m_Menu->Focused(true);
            }

            if (m_Menu->Visible() && !Active())
            {
                m_Menu->FocusedComponent(nullptr);
                for (auto& i : m_Menu->Components())
                    i->Focused(false);
            }
            m_Menu->Visible(Active());
            if (A == Align::CENTER || A == Align::BOTTOM)
                m_Menu->Position({ X(), Y() - m_Menu->Height() });

            else if (A == Align::LEFT)
                m_Menu->Position({ X() - m_Menu->Width(), Y() + Height() - m_Menu->Height() });

            else
                m_Menu->Position({ X() + Width(), Y() + Height() - m_Menu->Height() });

            ButtonType::Update(viewport);
        }

        /**
         * Emplace an item to the <code>Menu</code>
         * @return reference to the emplaced item
         * @tparam T type
         * @param ...args constructor arguments for <code>T</code>
         */
        template<typename T, typename ...Args>
        T& Emplace(Args&&... args)
        {
            return m_Menu->Emplace<T>(std::forward<Args>(args)...);
        }

        /**
         * Get the menu of this menu button
         * @return menu
         */
        MenuBase& MenuBase() { return *m_Menu; }

        bool WithinBounds(const Vec2<int>& pos) const override
        {
            return ButtonType::WithinBounds(pos) || (Active() && m_Menu->WithinBounds(pos));
        }

    private:
        ::MenuBase* m_Menu;

        bool m_CloseMenu = false, m_OpenMenu = false;
    };
}