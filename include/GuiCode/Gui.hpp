#pragma once
#include "GuiCode/ui/window/WindowBase.hpp"
#include "GuiCode/ui/RightClickMenu.hpp"
#include "GuiCode/ui/Dragging.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// -------------------------------- Gui -------------------------------------
// --------------------------------------------------------------------------

class Gui
{
public:
	Gui()
	{
		//RightClickMenu::Get().Hide();
		//Dragging::Get().Hide();
	}

	/**
	 * Add a new Window to the Gui
	 * @return reference to the added Window
	 * @tparam T the type of Window
	 * @param ...args arguments to initialize <code>T</code>
	 */
	template<typename T, typename ...Args>
	T& AddWindow(Args...args)
	{
		auto& _w = m_Windows.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		m_Init();
		return *dynamic_cast<T*>(_w.get());;
	}

	/**
	 * Call this in a while loop
	 * <code> while (gui.Loop);</code>
	 * @return true when Gui is running
	 */
	bool Loop()
	{


		LIMIT_FPS(60);
		for (auto& _w : m_Windows)
		{
			_w->Loop();

			if (_w->ShouldClose())
			{
				_w->ShouldClose(false);
				if (_w->HideOnClose())
					_w->Hide();
				else
					m_Running = false;
			}
		}

		RightClickMenu::Get().Loop();
		//Dragging::Get().Loop();

		return m_Running;
	}

	/**
	 * Close the gui.
	 */
	void Close()
	{
		m_Running = false;
	}

private:

	void m_Init()
	{
		static bool _init = false;
		if (!_init)
		{
			Graphics::Init();
			RightClickMenu::Get().Hide();
			//Dragging::Get().Hide();
		}
		_init = true;
	}


	std::vector<std::unique_ptr<WindowBase>> m_Windows;
	bool m_Running = true;
};
