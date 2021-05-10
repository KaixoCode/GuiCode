#include "GuiCode/ui/components/menus/Menu.hpp"
#include "GuiCode/ui/components/buttons/Button.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Menu Base -----------------------------------
// --------------------------------------------------------------------------

MenuBase::MenuBase()
    : Container()
{
    m_Listener += [this](Event::KeyPressed& e) 
    {
        if (!Focused())
            return;

        if (Focused() && e.key == Key::DOWN)
        {
            if (FocusedComponent() == nullptr && Components().size() > 0)
            {
                ButtonBase* pre = dynamic_cast<ButtonBase*>(Components()[0].get());
                while (pre && pre->Disabled())
                {
                    pre = pre->PostButton();
                }

                if (pre)
                {
                    pre->Focused(true);
                }
            }
        }

        if (Focused() && e.key == Key::UP)
        {
            if (FocusedComponent() == nullptr && Components().size() > 0)
            {
                ButtonBase* post = dynamic_cast<ButtonBase*>(Components()[Components().size() - 1].get());
                while (post && post->Disabled())
                {
                    post = post->PreButton();
                }

                if (post)
                {
                    post->Focused(true);
                }
            }
        }
    };
};

bool MenuBase::WithinBounds(const Vec2<int>& pos) const
{
    bool _within = Component::WithinBounds(pos);
    for (auto& _i : m_Components)
        _within |= _i->WithinBounds(pos);
    return _within;
}