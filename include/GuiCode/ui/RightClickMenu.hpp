#pragma once
#include "GuiCode/ui/window/MenuFrame.hpp"

// --------------------------------------------------------------------------
// ------------------------- Right Click Menu -------------------------------
// --------------------------------------------------------------------------

class RightClickMenu;
class RightClickMenu : public MenuFrame
{
public:

	// Get the instance of this singleton
	static RightClickMenu& Get()
	{
		static RightClickMenu _instance;
		return _instance;
	}

	void Loop() override
	{
		// Hide this window when it loses focus.
		if (GetForegroundWindow() != GetWin32Handle())
			Hide();

		MenuFrame::Loop();
	}

	void Close()
	{
		SetMenu(nullptr);
		Hide();
	}

	void Open(MenuBase* menu)
	{
		// TODO: abstract this OS specific code away

		// Change the capture to this window to make sure 
		// when releasing the mouse it doesn't immediatly
		// unfocus this window and close it.
		ReleaseCapture();
		SetCapture(Get().GetWin32Handle());

		// Set the menu
		SetMenu(menu);

		// Show the window at the cursor position
		POINT point; GetCursorPos(&point);
		Location({ point.x, point.y });
		Show();
	}

private:
	RightClickMenu()
		: MenuFrame("", 50, 50)
	{}
};