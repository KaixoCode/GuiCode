#include "GuiCode/ui/components/panels/ZoomPanel.hpp"

ZoomPanel::ZoomPanel()
{
	m_Listener += [this](Event::MouseWheel& event)
	{
		if (event.wheelmod & Event::Mod::CONTROL)
		{
			m_MouseX = event.x;
			m_MouseY = event.y;

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

void ZoomPanel::Update(const Vec4<int>& viewport)
{
	// Smooth out the zooming
	double _r = 1.0;
	double _pzoom = m_Zoom;
	m_Zoom = m_Zoom * (1.0 - _r) + m_TargetZoom * _r;
	double _delta = m_Zoom / _pzoom;

	// Set the right size for the panel.
	if (m_EnableZoomX) Width(max((int)(m_RealWidth * GetZoom()), viewport.x + viewport.width));
	if (m_EnableZoomY) Height(max((int)(m_RealHeight * GetZoom()), viewport.y + viewport.height));

	// If we are scrolling in some other way, don't update the value in the
	// scrollbar but update the local values to stay up to date on those.
	if (m_ReadScrollValues || m_ScrollbarX && m_ScrollbarX->Hovering() || m_ScrollbarY && m_ScrollbarY->Hovering())
	{
		m_ReadScrollValues = false;

		if (m_ScrollbarX)
			m_TranslateX = m_ScrollbarX->Value();

		if (m_ScrollbarY)
			m_TranslateY = m_ScrollbarY->Value();
	}
	else
	{
		// If zoom in X direction is enabled, update the translate when zooming. 
		if (m_ScrollbarX && m_EnableZoomX)
		{
			// Calculate the target translate, update the mouse pos with the delta translate
			// and update its own position because in the next frame the pos will be updated
			// by the scrollpanel anyway.
			double _target = m_TranslateX - (m_MouseX) * (1.0 - _delta);
			m_MouseX = m_MouseX - m_TranslateX + _target;
			m_Pos.x = m_Pos.x + m_TranslateX - _target;

			// Constrain the translate to 0
			if (_target <= 0)
				_target = 0;

			// Update the actual values in the scrollbar, also update the range to the new
			// width of this panel to make sure it doesnt clip off any translating.
			m_TranslateX = _target;
			m_ScrollbarX->Range({ 0, (int)(Width() + (m_ScrollbarY ? m_ScrollbarY->Width() : 0)) });
			m_ScrollbarX->Value(m_TranslateX);
		}

		// If zoom in Y direction is enabled, update the translate when zooming. 
		if (m_ScrollbarY && m_EnableZoomY)
		{
			// Calculate the target translate, update the mouse pos with the delta translate
			// and update its own position because in the next frame the pos will be updated
			// by the scrollpanel anyway.
			double _target = m_ScrollbarY->Value() - (m_MouseY) * (1.0 - _delta);
			m_MouseY = m_MouseY - m_TranslateY + _target;
			m_Pos.y = m_Pos.y + m_TranslateY - _target;

			// Constrain the translate to 0
			if (_target <= 0)
				_target = 0;

			// Update the actual values in the scrollbar, also update the range to the new
			// width of this panel to make sure it doesnt clip off any translating.
			m_TranslateY = _target;
			m_ScrollbarY->Range({ 0, (int)(Height() + (m_ScrollbarX ? m_ScrollbarX->Height() : 0)) });
			m_ScrollbarY->Value(_target);
		}
	}
	Panel::Update(viewport);
}