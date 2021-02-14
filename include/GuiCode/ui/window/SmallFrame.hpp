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

class SmallFrame : public Window
{
public:
    SmallFrame(const std::string& name, const int width, const int height, bool hideonclose = false, bool r = true)
        : Window(name, width, height, r),
        m_CloseButton(&Emplace<Button<ButtonGraphics::Textured<>, ButtonType::Textured<ButtonType::Normal>>>(Graphics::Textures::Cross1, Graphics::Textures::Cross2, Graphics::Textures::Cross3, [&, hideonclose]() { if (hideonclose) this->Hide(); else this->Close(); }, "")),
        m_Panel(&Emplace<::Panel>())
    {}

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;
    ::Panel& Panel() { return *m_Panel; }

private:
    ::Panel* m_Panel;

    ButtonBase* m_CloseButton;
};