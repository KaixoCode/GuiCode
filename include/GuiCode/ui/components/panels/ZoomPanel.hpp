#pragma once
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/scrollbars/Scrollbar.hpp"

class ZoomPanel : public Panel
{
public:
	ZoomPanel();

	auto EnableZoom(bool x, bool y)        -> void { m_EnableZoomX = x, m_EnableZoomY = y; }
	auto ZoomRange(double min, double max) -> void { m_MinZoom = min, m_MaxZoom = max; }
	auto RealWidth(int w)                  -> void { m_RealWidth = w; }
	auto RealHeight(int h)                 -> void { m_RealHeight = h; }
	auto SetScrollbars(ScrollbarBase* x = nullptr, ScrollbarBase* y = nullptr) -> void { m_ScrollbarX = x, m_ScrollbarY = y; }
	
	double GetZoom() const { return m_Zoom; }

	void Update(const Vec4<int>& viewport) override;

private:
	double m_Zoom = 1,
		m_MinZoom = 0.1,
		m_MaxZoom = 64,
		m_TargetZoom = 1,
		m_MouseX = 0,
		m_MouseY = 0,
		m_TranslateX = 0,
		m_TranslateY = 0;

	int m_RealWidth = 0,
		m_RealHeight = 0;

	bool m_EnableZoomX = true, 
		m_EnableZoomY = true,
		m_ReadScrollValues = false;

	ScrollbarBase* m_ScrollbarX = nullptr;
	ScrollbarBase* m_ScrollbarY = nullptr;
};