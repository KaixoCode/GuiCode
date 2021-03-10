#pragma once
#include "GuiCode/ui/window/Window.hpp"
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/buttons/Button.hpp"
#include "GuiCode/ui/components/buttons/ButtonGraphics.hpp"
#include "GuiCode/ui/components/buttons/ButtonType.hpp"
#include "GuiCode/ui/components/menus/Menu.hpp"
#include "GuiCode/ui/components/menus/MenuGraphics.hpp"
#include "GuiCode/ui/components/menus/MenuType.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// ------------------------------- Frame ------------------------------------
// --------------------------------------------------------------------------

/**
 * A window with a menu and standard close/maximize/minimize buttons. Also has a base <code>Panel</code> to put content on.
 */
class Frame : public Window 
{
public:
    /**
     * Constructor
     * @param name name
     * @param width width
     * @param height height
     * @param hideonclose when true this window will not exit the program but simply hide the window when closed.
     * @param r resizeable
     */
    Frame(const std::string& name, const int width, const int height, bool hideonclose = false, bool show = true, bool r = true)
        : Window(name, width, height, hideonclose, show, r), m_Resizable(r),
        m_CloseButton(&Emplace<Button<ButtonGraphics::CloseButton, ButtonType::Normal>>([&]() { this->Close(); }, "")),
        m_MaxiButton1(&Emplace<Button<ButtonGraphics::Maxi1Button, ButtonType::Normal>>([&]() { this->Restore(); }, "")),
        m_MaxiButton2(&Emplace<Button<ButtonGraphics::Maxi2Button, ButtonType::Normal>>([&]() { this->Maximize(); }, "")),
        m_MinimButton(&Emplace<Button<ButtonGraphics::MiniButton, ButtonType::Normal>>([&]() { this->Iconify(); }, "")),
        m_Menu(&Emplace<::Menu<MenuGraphics::Horizontal, MenuType::ToggleHover>>()),
        m_Panel(&Emplace<::Panel>())
    {
        m_CloseButton->Size({ 46, 32 });
        m_MaxiButton1->Size({ 46, 32 });
        m_MaxiButton2->Size({ 46, 32 });
        m_MinimButton->Size({ 46, 32 });
    }

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;

    /**
     * Returns the menu.
     * @return menu
     */
    MenuBase& Menu() { return *m_Menu; }

    /**
     * Returns the base panel.
     * @return panel
     */
    ::Panel& Panel() { return *m_Panel; }

private:
    const bool m_Resizable;
    ::Panel* m_Panel;

    ButtonBase* m_CloseButton;
    ButtonBase* m_MaxiButton1;
    ButtonBase* m_MaxiButton2;
    ButtonBase* m_MinimButton;
    MenuBase* m_Menu;
};