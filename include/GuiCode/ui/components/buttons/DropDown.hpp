#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/buttons/ButtonType.hpp"
#include "GuiCode/ui/components/buttons/ButtonGraphics.hpp"
#include "GuiCode/ui/components/menus/MenuType.hpp"
#include "GuiCode/ui/components/menus/MenuGraphics.hpp"

template<typename Enum, typename Graphics = ButtonGraphics::Normal<>, typename MenuButtonGraphics = ButtonGraphics::Menu>
class DropDown;

// -------------------------------------------------------------------------- \\
// -------------------------- DropDown Option ------------------------------- \\
// -------------------------------------------------------------------------- \\

template<typename Enum, typename Graphics>
class DropDownOption : public Button<Graphics, ButtonType::List>
{
	using Callback = std::function<void(Enum)>;
public:
	template<typename T>
	DropDownOption(const std::string& name, Enum value, int key, T* parent, Callback e = [](Enum v) {})
		: Button<Graphics, ButtonType::List>([parent, name, value, e] { parent->SelectP(value); parent->Name(name); e(value); }, name, key),
		m_Value(value), m_Callback(e)
	{}
private:
	Enum Value() { return m_Value; }

	void Selected(bool v) { if (v && !m_Selected) m_Callback(m_Value); m_Selected = v; }
	bool Selected() { return m_Selected; }

	Callback m_Callback;
	Enum m_Value;

	template<typename A, typename B, typename C>
	friend class DropDown;
};

// -------------------------------------------------------------------------- \\
// ------------------------------ DropDown ---------------------------------- \\
// -------------------------------------------------------------------------- \\

/**
 * Simple <code>DropDown</code> button, opens a menu with options out of which you can select 1.
 */
template<typename Enum, typename Graphics = ButtonGraphics::Normal<>, typename MenuButtonGraphics = ButtonGraphics::Menu>
class DropDown : public ButtonType::Normal
{
public:
	using Callback = std::function<void(Enum)>;
	
	/**
	 * Constructor
	 * @param e callback
	 */
	DropDown(Callback e = [](Enum v) {})
		: m_Callback(e), ButtonType::Normal{ [this] { RightClickMenu::Get().Open(&m_Menu); } },
		m_Key(ButtonType::List::NewKey())
	{
		m_Menu.ButtonSize({ 140, 20 });
	}

	/**
	 * Add an option to the <code>DropDown</code>. It will automatically select the option if it is the 
	 * first one to be added.
	 * @param name name
	 * @param value value
	 * @param e callback specific for this option
	 */
	DropDownOption<Enum, MenuButtonGraphics>& AddOption(const std::string& name, Enum value, Callback e = [](Enum v) {})
	{
		auto& a = m_Menu.Emplace<DropDownOption<Enum, MenuButtonGraphics>>(name, value, m_Key, this, e);

		// Select if first component
		if (m_Menu.Components().size() == 1)
			a.Selected(true), m_Value = value, Name(name);

		return a;
	}

	/**
	 * Select the options with the given value.
	 * @param v value
	 */
	void Select(Enum v)
	{
		for (auto& i : m_Menu.Components())
		{
			auto _b = dynamic_cast<DropDownOption<Enum, MenuButtonGraphics>*>(i.get());
			if (_b)
				_b->Selected(false);
			if (_b && _b->Value() == v)
				_b->Selected(true), m_Value = v, Name(_b->Name());
		}
	}

	/**
	 * Get the current value of this <code>DropDown</code>.
	 * @return value
	 */
	Enum Value() { return m_Value; }

	/**
	 * Set the button size for the buttons in the menu that will be opened.
	 * @param s size
	 */
	void ButtonSize(const Vec2<int>& s) { m_Menu.ButtonSize(s); }

	/**
	 * Set the button width for the buttons in the menu that will be opened.
	 * @param w width
	 */
	void ButtonWidth(int w) { m_Menu.ButtonWidth(w); }

	/**
	 * Set the button height for the buttons in the menu that will be opened.
	 * @param h height
	 */
	void ButtonHeight(int h) { m_Menu.ButtonHeight(h); }

	/**
	 * Clear all options.
	 */
	void Clear()
	{
		m_Menu.Clear();
		ButtonType::Normal::Clear();
	}

	/**
	 * Get the menu.
	 */
	Menu<MenuGraphics::Vertical, MenuType::Normal>& Menu() { return m_Menu; }

	void Update(const Vec4<int>& v) override
	{
		ButtonType::Normal::Update(v);
	}

	void Render(CommandCollection& d) override
	{
		ButtonType::Normal::Render(d);
		Graphics::Render(*this, d);
	};

private:
	Callback m_Callback;
	void SelectP(Enum v) { m_Value = v; m_Callback(v); }
	friend class DropDownOption<Enum, MenuButtonGraphics>;
	int m_Key;
	Enum m_Value;
	::Menu<MenuGraphics::Vertical, MenuType::Normal> m_Menu;
};

