#pragma once
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/scrollbars/Scrollbar.hpp"

class ZoomPanel : public Panel
{
public:
	ZoomPanel()
	{
		m_Listener += [this](Event::MouseWheel& event)
		{
			if (event.wheelmod & Event::Mod::CONTROL)
			{
				m_MouseX = event.x;
				
				double _zoom = event.amount * 0.0008 + 1;
				m_TargetZoom *= _zoom;

				if (m_TargetZoom < m_MinZoom)
					_zoom = m_MinZoom / (m_TargetZoom / _zoom), m_TargetZoom = m_MinZoom;

				if (m_TargetZoom > m_MaxZoom)
					_zoom = m_MaxZoom / (m_TargetZoom / _zoom), m_TargetZoom = m_MaxZoom;
			}
		};

		m_Listener += [this](Event::MouseDragged& event)
		{
			m_ReadScrollValues = true;
		};
	}

	void EnableZoom(bool x, bool y)
	{
		m_EnableZoomX = x;
		m_EnableZoomY = y;
	}

	double GetZoom()
	{
		return m_Zoom;
	}

	void SetScrollbars(ScrollbarBase* x = nullptr, ScrollbarBase* y = nullptr)
	{
		m_ScrollbarX = x;
		m_ScrollbarY = y;
	}

	void Update(const Vec4<int>& viewport) override
	{
		double _r = 1.0;
		double _pzoom = m_Zoom;
		m_Zoom = m_Zoom * (1.0 - _r) + m_TargetZoom * _r;
		double _delta = m_Zoom / _pzoom;

		if (m_ReadScrollValues)
		{
			if (m_ScrollbarX)
				m_TargetTranslateX = m_ScrollbarX->Value();

			if (m_ScrollbarY)
				m_TargetTranslateY = m_ScrollbarY->Value();

			m_TranslateX = m_TargetTranslateX;
			m_TranslateY = m_TargetTranslateY;
		}
		else 
		{
			if (m_ScrollbarX && m_EnableZoomX)
			{
				m_TargetTranslateX = m_TranslateX - (m_MouseX) * (1.0 - _delta);
				m_MouseX = m_MouseX - m_TranslateX + m_TargetTranslateX;

				if (m_TargetTranslateX <= 0)
					m_TargetTranslateX = 0;

				m_TranslateX = m_TargetTranslateX;
				m_ScrollbarX->Range({ 0, Width() + 16 });
				m_ScrollbarX->Value(m_TranslateX);
			}

			if (m_ScrollbarY && m_EnableZoomY)
			{
				m_TargetTranslateY = m_ScrollbarY->Value() - ((double)m_MouseX) * (1.0 - _delta);

				if (m_TargetTranslateY <= 0)
					m_TargetTranslateY = 0;

				m_ScrollbarY->Range({ 0, Height() + 16 });
				m_ScrollbarY->Value(m_TargetTranslateY);
			}
		}

		Panel::Update(viewport);

		m_ReadScrollValues = false;
	}

	void Render(CommandCollection& d) override
	{
		using namespace Graphics;
		//d.Command<Scale>(m_Zoom);
		Panel::Render(d);
		//d.Command<Scale>(1.0);
	}

private:
	double m_Zoom = 1,
		m_MinZoom = 0.1,
		m_MaxZoom = 64,
		m_TargetZoom = 1,
		m_MouseWheelTotal = 0,
		m_MouseX = 0,
		m_TargetTranslateX = 0,
		m_TargetTranslateY = 0,
		m_TranslateX = 0,
		m_TranslateY = 0;

	int m_ZoomTimes = 0;

	bool m_EnableZoomX = true, 
		m_EnableZoomY = true,
		m_ReadScrollValues = false;

	ScrollbarBase* m_ScrollbarX;
	ScrollbarBase* m_ScrollbarY;
};