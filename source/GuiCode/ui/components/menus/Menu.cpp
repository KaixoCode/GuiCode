#include "GuiCode/ui/components/menus/Menu.hpp"

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
                Components()[0]->Focused(true);
        }

        if (Focused() && e.key == Key::UP)
        {
            if (FocusedComponent() == nullptr && Components().size() > 0)
                Components()[Components().size() - 1]->Focused(true);
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