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

		int biggestX = 0;
		int biggestY = 0;

		int sx = 0, bx = menu->Width();
		int sy = menu->Y(), by = menu->Y() + menu->Height();

		biggestX = (int)std::ceil(std::max((bx + 4) / m_Scale, 5.0f));
		biggestY = (int)std::ceil(std::max((by - sy + 5) / m_Scale, 5.0f));
		
		int x = 0;
		int y = 0;

		POINT point; GetCursorPos(&point);
		auto monitor = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);

		GetMonitorInfoA(monitor, &info);

		int maxx = info.rcMonitor.right;
		int maxy = info.rcMonitor.bottom;

		// Show at given x, y coords
		if (pos.x != -1 && pos.y != -1 && CurrentWindow)
		{
			RECT rect;
			GetWindowRect(CurrentWindow->GetWin32Handle(), &rect);
			x = rect.left + pos.x;
			y = rect.bottom - pos.y;
		}

		// Show the window at the cursor position
		else
		{
			x = point.x;
			y = point.y;
		}

		if (x + biggestX > maxx)
			x = maxx - biggestX;
		if (y + biggestY > maxy)
			y = maxy - biggestY;

		Location({ x, y });
		Size(menu->Size());
		m_PSize = menu->Size();
		Update(m_Viewport);
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