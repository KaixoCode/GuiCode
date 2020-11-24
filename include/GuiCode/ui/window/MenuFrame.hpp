#pragma once
#include "GuiCode/ui/window/Window.hpp"
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/menus/Menu.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu Frame Panel -------------------------------
// --------------------------------------------------------------------------

class MenuFramePanel : public Panel
{
public:
    MenuFramePanel();

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;

    auto SetMenu(MenuBase* menu) -> void { m_Menu = menu; }

private:
    MenuBase* m_Menu = nullptr;
};

// --------------------------------------------------------------------------
// ----------------------------- Menu Frame ---------------------------------
// --------------------------------------------------------------------------

class MenuFrame : public Window
{
public:
    MenuFrame(const std::string& name, const int width, const int height)
        : Window(name, width, height, true, false),
        m_Panel(&Emplace<::MenuFramePanel>())
    {}

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;
    
    ::Panel& Panel() { return *m_Panel; }

    void SetMenu(MenuBase* menu);

private:
    ::MenuFramePanel* m_Panel;
    MenuBase* m_Menu = nullptr;
};