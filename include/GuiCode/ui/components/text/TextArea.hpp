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
        m_Displayer.TextWrap(StringDisplayer::Wrap::Word);
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

    void  AlignLines(Align a) { m_Displayer.AlignLines(a); }
    void  Content(const std::string& c) { m_Displayer.Container().Content(c); }
    void  Editable(bool e) { m_Displayer.Container().Editable(e); }
    void  TextColor(Color c) { m_Displayer.TextColor(c); }
    void  SelectColor(Color c) { m_Displayer.SelectColor(c); }
    void  TextWrap(StringDisplayer::Wrap wrap) { m_Displayer.TextWrap(wrap); }
    void  LineHeight(int h) { m_Displayer.LineHeight(h); };
    void  Font(int f, float size) { m_Displayer.Font(f, size); };
    void  Padding(int p) { m_Displayer.Padding(p); };
    auto  AlignLines() -> Align { return m_Displayer.AlignLines(); }
    auto  Content() -> std::string& { return m_Displayer.Container().Content(); }
    bool  Editable() { return m_Displayer.Container().Editable(); }
    auto  TextColor() const -> Color { return m_Displayer.TextColor(); }
    auto  SelectColor() const -> Color { return m_Displayer.SelectColor(); }
    auto  TextWrap() -> StringDisplayer::Wrap { return m_Displayer.TextWrap(); }
    int   LineHeight() { return m_Displayer.LineHeight(); };
    int   Font() { return m_Displayer.Font(); };
    float FontSize() { return m_Displayer.FontSize(); };
    int   Padding() { return m_Displayer.Padding(); };

private:
    StringDisplayer& m_Displayer;
    Vec2<int> m_Mouse;

    void UpdateScroll()
    {
        auto pos = m_Displayer.IndexToPosition(m_Displayer.Container().Selection().start);
        pos.y = m_Displayer.Height() - pos.y;

        if (pos.y > m_ScrollbarY->Value() + Height() - m_Displayer.Padding() * 2 - m_Displayer.LineHeight()) {
            m_ScrollbarY->Value(pos.y - Height() + m_Displayer.Padding() * 2 + m_Displayer.LineHeight());
        }
        else if (pos.y < m_ScrollbarY->Value() + m_Displayer.Padding()) {
            m_ScrollbarY->Value(pos.y - m_Displayer.Padding());
        }

        if (pos.x > m_ScrollbarX->Value() + Width() - m_Displayer.Padding() * 2) {
            m_ScrollbarX->Value(pos.x - Width() + m_Displayer.Padding() * 2);
        }
        else if (pos.x < m_ScrollbarX->Value() + m_Displayer.Padding()) {
            m_ScrollbarX->Value(pos.x - m_Displayer.Padding());
        }
    }
};