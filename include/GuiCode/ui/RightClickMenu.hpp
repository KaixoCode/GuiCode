#pragma once
#include "GuiCode/ui/window/MenuFrame.hpp"

// --------------------------------------------------------------------------
// ------------------------- Right Click Menu -------------------------------
// --------------------------------------------------------------------------

/**
 * Right click menu class, uses a <code>ComponentFrame</code> to display a <code>Menu</code> in 
 * a separate <code>Window</code>.
 */
class RightClickMenu : public ComponentFrame
{
public:

	/**
	 * Get the instance (Singleton).
	 * @return instance
	 */
	static RightClickMenu& Get()
	{
		static RightClickMenu _instance;
		return _instance;
	}

	/**
	 * Don't worry about this, the <code>Gui</code> class will take care of this.
	 */
	void Loop() override
	{
		// Hide this window when it loses focus.
		if (GetForegroundWindow() != GetWin32Handle() && m_Focus)
			Close();

		ComponentFrame::Loop();
		
		if (m_Close)
			Close();
	}

	/**
	 * Close the right click menu.
	 */
	void Close()
	{
		m_Focus = false;
		m_Close = false;
		Component(nullptr);
		Hide();
	}

	/**
	 * Open the right click menu.
	 * @param menu menu to show
	 */
	void Open(MenuBase* menu, bool f = false)
	{
		m_Focus = true;

		// TODO: abstract this OS specific code away
		
		// Set the menu
		Component(menu);

		// Show the window at the cursor position
		POINT point; GetCursorPos(&point);
		Location({ point.x, point.y });
		Show();
		if (f)
			SetForegroundWindow(GetWin32Handle());

		// Change the capture to this window to make sure 
		// when releasing the mouse it doesn't immediatly
		// unfocus this window and close it.
		ReleaseCapture();
		SetCapture(GetWin32Handle());
	}

	bool Opened() const { return m_Focus; }

private:
	bool m_Pressed = false; 
	bool m_Close = false;
	bool m_Focus = false;

	RightClickMenu()
		: ComponentFrame("", 50, 50)
	{
		m_Listener += [this](Event::MousePressed& e)
		{
			m_Pressed = true;
		};

		m_Listener += [this](Event::MouseReleased& e)
		{
			if (m_Pressed)
				m_Close = true;

			m_Pressed = false;
		};
	}
};