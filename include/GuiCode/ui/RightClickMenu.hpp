#pragma once
#include "GuiCode/ui/window/MenuFrame.hpp"

// --------------------------------------------------------------------------
// ------------------------- Right Click Menu -------------------------------
// --------------------------------------------------------------------------

class RightClickMenu;
class RightClickMenu : public ComponentFrame
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

		ComponentFrame::Loop();
	}

	void Close()
	{
		Component(nullptr);
		Hide();
	}

	void Open(MenuBase* menu)
	{
		// TODO: abstract this OS specific code away

		// Set the menu
		Component(menu);

		// Show the window at the cursor position
		POINT point; GetCursorPos(&point);
		Location({ point.x, point.y });
		Show();

		// Change the capture to this window to make sure 
		// when releasing the mouse it doesn't immediatly
		// unfocus this window and close it.
		ReleaseCapture();
		SetCapture(Get().GetWin32Handle());
	}

private:
	bool m_Pressed = false; 

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
				Close();

			m_Pressed = false;
		};
	}
};