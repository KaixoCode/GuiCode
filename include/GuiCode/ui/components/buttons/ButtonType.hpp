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
    // ---------- Normal button behaviour, press to trigger callback ------------
    // --------------------------------------------------------------------------

    class Normal : public ButtonBase
    {
    public:
        Normal(Callback c = []() {}, const std::string& name = "Button", 
            Vec2<int> size = { 70, 40 }, Key key = Key::NONE);
    
    private:
        Callback m_Callback;
    };

    // --------------------------------------------------------------------------
    // -------------------------------- List ------------------------------------
    // --------------------------------------------------------------------------
    // ---------- Normal button behaviour, press to trigger callback ------------
    // --------------------------------------------------------------------------

    class List : public Normal
    {
    public:
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
                        LOG(Selected());
                    }
                }
            };

            auto& _exists = m_Lists.find(id);
            if (_exists == m_Lists.end())
                m_Lists.emplace(id, std::vector<List*>());
   
            m_Lists[id].push_back(this);
        };

        void Selected(bool b) { m_Selected = b; }
        bool Selected() { return m_Selected; }

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
    // - Clicking the button toggles the active state, can be linked to a bool --
    // --------------------------------------------------------------------------

    class Toggle : public ButtonBase
    {
    public:
        Toggle(bool* a, const std::string& name = "Button",
            Vec2<int> size = { 70, 40 }, Key key = Key::NONE);

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
    // ---------- Same as Toggle, but de-activates when it loses focus ----------
    // --------------------------------------------------------------------------

    class FocusToggle : public Toggle
    {
    public:
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
    // -------------------- Activates when you hover over it --------------------
    // --------------------------------------------------------------------------

    class Hover : public ButtonBase
    {
    public:
        Hover(const std::string& name = "Button", Vec2<int> size = { 70, 40 });
    };

    // --------------------------------------------------------------------------
    // ------------------------------- Textured ---------------------------------
    // --------------------------------------------------------------------------
    // ---- Special Type that stores 3 textures for the 3 states of a button ----
    // --------------------------------------------------------------------------

    template<typename ButtonType = ButtonBase>
    class Textured : public ButtonType
    {
    public:
        Graphics::Texture& m_Texture1;
        Graphics::Texture& m_Texture2;
        Graphics::Texture& m_Texture3;

        template<typename ...Args>
        Textured(Graphics::Texture& t1, Graphics::Texture& t2, Graphics::Texture& t3, Args&& ...a)
            : m_Texture1{ t1 }, m_Texture2{ t2 }, m_Texture3{ t3 }, ButtonType(std::forward<Args>(a)...)
        {}
    };

    // --------------------------------------------------------------------------
    // --------------------------------- Menu -----------------------------------
    // --------------------------------------------------------------------------
    // ---- Button that stores a menu that will be opened when clicked on it ----
    // --------------------------------------------------------------------------

    template<typename Graphics, typename MenuType = MenuBase, typename ButtonType = Toggle, Align A = Align::BOTTOM>
    class Menu : public ButtonType
    {
    public:

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