#pragma once
#include "GuiCode/ui/window/ComponentFrame.hpp"

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
	void Close() override
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
	void Open(MenuBase* menu, bool f = false, const Vec2<int>& pos = { -1, -1 })
	{
		m_Focus = true;

		// TODO: abstract this OS specific code away
		
		// Set the menu
		Component(menu);

		// Show the window at the cursor position
		if (pos.x != -1 && pos.y != -1 && m_CurrentWindow)
		{
			RECT rect;
			GetWindowRect(m_CurrentWindow->GetWin32Handle(), &rect);
			Location({ rect.left + pos.x, rect.bottom - pos.y });
		}
		else
		{
			POINT point; GetCursorPos(&point);
			Location({ point.x, point.y });
		}
		Size(menu->Size());
		m_PSize = menu->Size();
		SetForegroundWindow(GetWin32Handle());

		// Change the capture to this window to make sure 
		// when releasing the mouse it doesn't immediatly
		// unfocus this window and close it.
		ReleaseCapture();
		SetCapture(GetWin32Handle());
		Visible(true);
	}

	bool Opened() const { return m_Focus; }

private:
	bool m_Pressed = false; 
	bool m_Close = false;
	bool m_Focus = false;

	RightClickMenu()
		: ComponentFrame(WindowData{ "", Vec2<int>{ 50, 50 }, true, true, true, false, false, false, nullptr })
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