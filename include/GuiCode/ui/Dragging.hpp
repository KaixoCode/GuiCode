#pragma once
#include "GuiCode/ui/window/ComponentFrame.hpp"

// --------------------------------------------------------------------------
// ------------------------- Right Click Menu -------------------------------
// --------------------------------------------------------------------------

class Dragging;
class Dragging : public ComponentFrame
{
public:

	// Get the instance of this singleton
	static Dragging& Get()
	{
		static Dragging _instance;
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

	void Start(::Component* c, Vec2<int> pos)
	{
		// TODO: abstract this OS specific code away

		// Change the capture to this window to make sure 
		// when releasing the mouse it doesn't immediatly
		// unfocus this window and close it.
		ReleaseCapture();
		SetCapture(Get().GetWin32Handle());

		// Set the menu
		Component(c);

		// Show the window at the cursor position
		POINT point; GetCursorPos(&point);
		m_StartX = pos.x;
		m_StartY = pos.y;
		Location({ point.x - m_StartX, point.y - m_StartY });
		Show();
	}

private:
	Dragging()
		: ComponentFrame({})
	{
		m_Listener += [this](Event::MouseMoved& e)
		{
			POINT point; GetCursorPos(&point);
			Location({ point.x - m_StartX, point.y - m_StartY });
		};

		m_Listener += [this](Event::MousePressed& e)
		{
			Component(nullptr);
			Hide();
		};

		m_Listener += [this](Event::MouseReleased& e)
		{
			Component(nullptr);
			Hide();
		};
	}

	int m_StartX = 0,
		m_StartY = 0;

};