#pragma once
#include "GuiCode/ui/components/panels/ScrollPanel.hpp"
#include "GuiCode/ui/components/text/StringDisplayer.hpp"

class TextArea : public ScrollPanel
{
public:
	TextArea()
		: m_Displayer(Panel<StringDisplayer>())
	{

        m_Listener += [this](Event::MousePressed& e)
        {
            if (e.button != Event::MouseButton::LEFT)
                return;

            m_Mouse = { e.x, e.y };
        };

        m_Listener += [this](Event::MouseDragged& e)
        {
            if (e.button != Event::MouseButton::LEFT)
                return;

            m_Mouse = { e.x, e.y };
        };

        m_Listener += [this](Event::KeyPressed& e)
        {
            UpdateScroll();
        };
        
        m_Listener += [this](Event::KeyTyped& e)
        {
            UpdateScroll();
        };

        Background(Color{ 255, 255, 255, 255 });
        EnableScrollbars(false, true);
	}


    void Update(const Vec4<int>& v)
    {
        ScrollPanel::Update(v);

        if (m_Displayer.Dragging())
        {
            if (m_EnableY && m_Mouse.y < Height() * 0.1)
                m_ScrollbarY->Scroll(5 * 0.02 * ((Height() * 0.1 - m_Mouse.y)));

            if (m_EnableY && m_Mouse.y > Height() - Height() * 0.1)
                m_ScrollbarY->Scroll(-5 * 0.02 * ((m_Mouse.y - (Height() - Height() * 0.1))));

            if (m_EnableX && m_Mouse.x < Width() * 0.1)
                m_ScrollbarX->Scroll(-5 * 0.02 * ((Width() * 0.1 - m_Mouse.x)));

            if (m_EnableX && m_Mouse.x > Width() - Width() * 0.1)
                m_ScrollbarX->Scroll(5 * 0.02 * ((m_Mouse.x - (Width() - Width() * 0.1))));
        }
    }


private:
	StringDisplayer& m_Displayer;
    Vec2<int> m_Mouse;

    void UpdateScroll()
    {
        auto pos = m_Displayer.IndexToPosition(m_Displayer.Container().Selection().start);
        pos.y = m_Displayer.Height() - pos.y;

        if (pos.y > m_ScrollbarY->Value() + Height() - m_Displayer.m_Padding * 2 - m_Displayer.m_FontSize) {
            m_ScrollbarY->Value(pos.y - Height() + m_Displayer.m_Padding * 2 + m_Displayer.m_FontSize);
        }
        else if (pos.y < m_ScrollbarY->Value() + m_Displayer.m_Padding) {
            m_ScrollbarY->Value(pos.y - m_Displayer.m_Padding);
        }

        if (pos.x > m_ScrollbarX->Value() + Width() - m_Displayer.m_Padding * 2) {
            m_ScrollbarX->Value(pos.x - Width() + m_Displayer.m_Padding * 2);
        }
        else if (pos.x < m_ScrollbarX->Value() + m_Displayer.m_Padding) {
            m_ScrollbarX->Value(pos.x - m_Displayer.m_Padding);
        }
    }
};