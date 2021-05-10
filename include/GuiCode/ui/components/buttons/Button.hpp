#pragma once
#include "GuiCode/ui/components/Container.hpp"

using Callback = std::function<void()>;
using ToggleCallback = std::function<void(bool&)>;

// --------------------------------------------------------------------------
// ---------------------------- Button Base ---------------------------------
// --------------------------------------------------------------------------

/**
 * Basis for a <code>Button</code>
 */
class ButtonBase : public Container
{
public:

    /**
     * Constructor
     * @param name name
     * @param size size
     * @param key keycombo
     */
    ButtonBase(const std::string& name = "Button", Key key = Key::NONE)
        : m_KeyCombo(key), m_Name(name), Container()
    {}

    /**
     * Enable this <code>Button</code>.
     */
    virtual void Enable() { m_Enabled = true; }

    /**
     * Disable this <code>Button</code>.
     */
    virtual void Disable() { m_Enabled = false; }

    /**
     * Activate this <code>Button</code>; either means pressed or toggled on depending on type.
     * @param a active
     */
    virtual void Active(bool a) { m_Active = a; }
    
    /**
     * Set the name of <code>Button</code>.
     * @param a the new name
     */
    virtual void Name(const std::string& a) { m_Name = a; }

    /**
     * True when this <code>Button</code> is enabled.
     */
    virtual bool Enabled() const { return m_Enabled; }

    /**
     * Set if this button is enabled.
     * @param v enabled
     */
    virtual void Enabled(bool v) { m_Enabled = v; }
   
    /**
     * True when this <code>Button</code> is disabled.
     */
    virtual bool Disabled() const { return !m_Enabled; }

    /**
     * True when this <code>Button</code> is active.
     */
    virtual bool Active() const { return m_Active; }

    /**
     * Returns the keycombo for 'pressing' this <code>Button</code>.
     * @return keycombo
     */
    Key KeyCombo() const { return m_KeyCombo; }
    
    /**
     * Returns the name of this <code>Button</code>.
     * @return name
     */
    const std::string& Name() const { return m_Name; }

    virtual bool HasSubMenu() { return false; }

    ButtonBase* PreButton() { return m_Pre; }
    ButtonBase* PostButton() { return m_Post; }

protected:
    bool m_Active = false,
        m_Enabled = true,
        m_InMenu = false,
        m_Clip = true;

    ButtonBase* m_Pre = nullptr, * m_Post = nullptr;

    Key m_KeyCombo;
    std::string m_Name;

    // Used for using arrows in menus 
    friend class MenuBase;
    void InMenu(bool v) { m_InMenu = v; }
    void PreButton(ButtonBase* v) { m_Pre = v; }
    void PostButton(ButtonBase* v) { m_Post = v; }
};

// --------------------------------------------------------------------------
// ------------------------------ Button ------------------------------------
// --------------------------------------------------------------------------

/**
 * Templated <code>Button</code> class, allows fully customized behaviour and graphics.
 * @tparam ButtonGraphics a class with a static Render method that takes in the <code>Button</code> and the <code>CommandCollection</code>.
 * @tparam ButtonType the type of button, must be a class that inherits ButtonBase.
 */
template<typename ButtonGraphics, typename ButtonType = ButtonBase>
struct Button : public ButtonType
{
    using ButtonType::ButtonType;

    void Render(CommandCollection& d) override
    {
        if (m_Clip)
            d.Command<Graphics::Clip>(Vec4<int>{ Position(), Size() });
        ButtonType::Render(d);
        ButtonGraphics::Render(*this, d);
        if (m_Clip)
            d.Command<Graphics::PopClip>();
    };
};

