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

/**
 * Smaller version of <code>Frame</code>, only has a close button and no menu.
 */
class SmallFrame : public Window
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
    SmallFrame(const std::string& name, const int width, const int height, bool hideonclose = false, bool r = true)
        : Window(name, width, height, hideonclose, r),
        m_CloseButton(&Emplace<Button<ButtonGraphics::CloseButton, ButtonType::Normal>>([&]() { this->Close(); }, "")),
        m_Panel(&Emplace<::Panel>())
    {
        m_CloseButton->Size({ 46, 32 });
    }

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;
   
    /**
     * Returns the base panel.
     * @return panel
     */
    ::Panel& Panel() { return *m_Panel; }

private:
    ::Panel* m_Panel;

    ButtonBase* m_CloseButton;
};