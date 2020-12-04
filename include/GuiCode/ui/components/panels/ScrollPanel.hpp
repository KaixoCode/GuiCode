#pragma once
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/panels/ZoomPanel.hpp"
#include "GuiCode/ui/components/scrollbars/Scrollbar.hpp"
#include "GuiCode/ui/components/scrollbars/ScrollbarType.hpp"
#include "GuiCode/ui/components/scrollbars/ScrollbarGraphics.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Scroll Panel --------------------------------
// --------------------------------------------------------------------------

class ScrollPanel : public Panel
{
public:
    ScrollPanel();

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;

    void EnableScrollbars(bool x, bool y) { m_EnableX = x, m_EnableY = y; }

    Vec2<int> Scrolled() const { return { m_ScrollbarX->Value(), m_ScrollbarY->Value() }; }

    ::Panel& Component() const { return *m_Panel; }

    template<typename T, typename ...Args>
    T& Component(Args&&... args)
    {
        if (m_Panel != nullptr)
            return dynamic_cast<T&>(*m_Panel);

        auto& _t = Emplace<T>(std::forward<Args>(args)...);
        m_Panel = &_t;
        m_ScrollbarX = &Emplace<Scrollbar<ScrollbarGraphics::Normal, ScrollbarType::Horizontal>>();
        m_ScrollbarY = &Emplace<Scrollbar<ScrollbarGraphics::Normal, ScrollbarType::Vertical>>();
        
        if constexpr (std::is_base_of<ZoomPanel, T>::value)
            _t.SetScrollbars(m_ScrollbarX, m_ScrollbarY);
        
        return _t;
    }

protected:
    ScrollbarBase* m_ScrollbarX{ 0 };
    ScrollbarBase* m_ScrollbarY{ 0 };
    ::Panel* m_Panel{ 0 };

    bool m_EnableX = true,
        m_EnableY = true;

    int m_PMouseX = 0, 
        m_PMouseY = 0,
        m_PValX = 0,
        m_PValY = 0;
};
