#include "GuiCode/ui/window/WindowBase.hpp"



class Gui
{
public:
	template<typename T, typename ...Args>
	T& AddWindow(Args...args)
	{
		auto& _w = m_Windows.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		static bool _init = false;
		if (!_init)
			Graphics::Init();
		_init = true;
		return *dynamic_cast<T*>(_w.get());;
	}

	bool Loop()
	{
		bool _running = true;

		LIMIT_FPS(60);
		for (auto& _w : m_Windows)
		{
			_w->Loop();

			if (_w->ShouldClose())
				_running = false;
		}

		return _running;
	}

private:
	std::vector<std::unique_ptr<WindowBase>> m_Windows;

};