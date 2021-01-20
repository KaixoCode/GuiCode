#pragma once
#include "GuiCode/ui/window/Window.hpp"
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/menus/Menu.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu Frame Panel -------------------------------
// --------------------------------------------------------------------------

class ComponentFramePanel : public Panel
{
public:
    ComponentFramePanel();

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;

    auto Component(::Component* c) -> void { m_Component = c; }

private:
    ::Component* m_Component = nullptr;
};

// --------------------------------------------------------------------------
// ----------------------------- Menu Frame ---------------------------------
// --------------------------------------------------------------------------

class ComponentFrame : public Window
{
public:
    ComponentFrame(const std::string& name, const int width, const int height)
        : Window(name, width, height, true, false),
        m_Panel(&Emplace<::ComponentFramePanel>())
    {}

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;
    
    ::Panel& Panel() { return *m_Panel; }

    void Component(::Component* c);
    ::Component* Component() const { return m_Component; }
        
private:
    ::ComponentFramePanel* m_Panel;
    ::Component* m_Component = nullptr;
};