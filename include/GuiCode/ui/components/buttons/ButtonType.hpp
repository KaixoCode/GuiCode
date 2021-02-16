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
        Normal(Callback c = []() {}, const std::string& name = "Button", 
            Vec2<int> size = { 70, 40 }, Key key = Key::NONE);
    
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
         * @param size size
         * @param id list id
         * @param key keycombo
         */
        List(Callback c = []() {}, const std::string& name = "Button",
            Vec2<int> size = { 70, 40 }, int id = 0, Key key = Key::NONE)
            : Normal(c, name, size, key), m_Id(id)
        {
            m_Listener += [this](Event::MouseReleased& event)
            {
                if (event.button == Event::MouseButton::LEFT)
                {
                    if (!Disabled() && Component::WithinBounds({ event.x, event.y }))
                    {
                        for (auto& _l : m_Lists[m_Id])
                            _l->Selected(false);
                        
                        Selected(true);
                    }
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
        void Selected(bool b) { m_Selected = b; }

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
            int key = std::rand();
            while (m_Lists.find(key) != m_Lists.end())
                key = std::rand();

            return key;
        }

    private:
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
            Vec2<int> size = { 70, 40 }, Key key = Key::NONE);

        /**
         * Constructor
         * @param name name
         * @param size size
         * @param key keycombo
         */
        Toggle(const std::string& name = "Button",
            Vec2<int> size = { 70, 40 }, Key key = Key::NONE);

        auto Active(bool a) -> void override { if (m_Link) *m_Link = a; m_Active = a; }
        bool Active() const override { return m_Active; }

        void Update(const Vec4<int>& viewport) override;

    protected:
        bool* m_Link = { 0 };

        virtual void SetupCallbacks();
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
        Hover(const std::string& name = "Button", Vec2<int> size = { 70, 40 });
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
        {}

        void Update(const Vec4<int>& viewport) override
        {
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

        bool WithinBounds(const Vec2<int>& pos) const override
        {
            return ButtonType::WithinBounds(pos) || (Active() && m_Menu->WithinBounds(pos));
        }

    private:
        MenuBase* m_Menu;
    };
}