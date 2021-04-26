#pragma once
#include "GuiCode/ui/window/Window.hpp"
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/menus/Menu.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu Frame Panel -------------------------------
// --------------------------------------------------------------------------

/**
 * Panel used by the <code>ComponentFrame</code>.
 */
class ComponentFramePanel : public Panel
{
public:
    /**
     * Constructor
     */
    ComponentFramePanel();

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;

    /**
     * Set the component of this <code>ComponentFramePanel</code>.
     * @param c component
     */
    auto Component(::Component* c) -> void { m_Component = c; }

private:
    ::Component* m_Component = nullptr;
};

// --------------------------------------------------------------------------
// ----------------------------- Menu Frame ---------------------------------
// --------------------------------------------------------------------------

/**
 * A frame to display a single component, used by the RightClickMenu.
 */
class ComponentFrame : public Window
{
public:
    /**
     * Constructor
     * @param name name
     * @param width width
     * @param height height
     */
    ComponentFrame(const std::string& name, const int width, const int height)
        : Window(name, width, height, true, false, true, false),
        m_Panel(&Emplace<::ComponentFramePanel>())
    {
        m_Listener += [this](Event::KeyPressed& e)
        {
            if (e.key == Key::ESC && Visible())
                Close();
        };
    }

    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection& d) override;
    
    /**
     * Returns the base panel.
     * @return panel
     */
    ::Panel& Panel() { return *m_Panel; }

    /**
     * Set the component of this <code>ComponentFrame</code>.
     * @param c component
     */
    void Component(::Component* c);

    /**
     * Returns a pointer to the component.
     * @return pointer to the component
     */
    ::Component* Component() const { return m_Component; }


    virtual void Close() = 0;
        
private:
    ::ComponentFramePanel* m_Panel;
    ::Component* m_Component = nullptr;
};