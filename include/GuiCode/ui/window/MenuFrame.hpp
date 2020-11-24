#include "GuiCode/ui/window/Window.hpp"
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/menus/Menu.hpp"

class MenuFrame : public Window
{
public:
    MenuFrame(const std::string& name, const int width, const int height)
        : Window(name, width, height, true, false),
        m_Panel(&Emplace<::Panel>())
    {}

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;
    ::Panel& Panel() { return *m_Panel; }

private:
    ::Panel* m_Panel;

    //MenuBase* m_Menu;
};