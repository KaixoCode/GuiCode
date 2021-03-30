#pragma once
#include "GuiCode/ui/components/panels/ScrollPanel.hpp"
#include "GuiCode/ui/components/text/StringDisplayer.hpp"

class TextBox : public Panel
{
public:
    TextBox()
        : m_Displayer(Emplace<StringDisplayer>())
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
        m_Displayer.TextWrap(StringDisplayer::Wrap::None);
    }

    void Update(const Vec4<int>& v)
    {
        m_Displayer.LineHeight(Height());
        Panel::Update(v);

        m_Displayer.Position({ -m_ScrollX, 0 });
        m_Displayer.Height(Height());

        m_Displayer.MinWidth(Width());

        if (m_Displayer.Dragging() && m_Displayer.Width() - m_Displayer.Padding() * 2 > Width())
        {
            if (m_Mouse.x < Width() * 0.1)
                m_ScrollX += -5 * 0.02 * ((Width() * 0.1 - m_Mouse.x));

            if (m_Mouse.x > Width() - Width() * 0.1)
                m_ScrollX += 5 * 0.02 * ((m_Mouse.x - (Width() - Width() * 0.1)));
        }

        m_ScrollX = constrain(m_ScrollX, 0, std::max(m_Displayer.Width() - Width(), 0));
    }

    void Render(CommandCollection& d)
    {
        using namespace Graphics;
        d.Command<Clip>(X(), Y(), Width(), Height());
        d.Command<PushMatrix>();
        d.Command<Translate>(Vec2<int>{ X(), Y() });
        Background(d);
        m_Displayer.Render(d);
        d.Command<PopMatrix>();
        d.Command<PopClip>();
        NeedsRedraw(false);
        Component::Render(d);
    }
    
    void  AlignLines(Align a) { m_Displayer.AlignLines(a); }
    void  Content(const std::string& c) { m_Displayer.Container().Content(c); }
    void  Editable(bool e) { m_Displayer.Container().Editable(e); }
    void  TextColor(Color c) { m_Displayer.TextColor(c); }
    void  SelectColor(Color c) { m_Displayer.SelectColor(c); }
    void  Font(int f, float size) { m_Displayer.Font(f, size); };
    void  Padding(int p) { m_Displayer.Padding(p); };
    auto  AlignLines() -> Align { return m_Displayer.AlignLines(); }
    auto  Content() -> std::string& { return m_Displayer.Container().Content(); }
    bool  Editable() { return m_Displayer.Container().Editable(); }
    auto  TextColor() const -> Color { return m_Displayer.TextColor(); }
    auto  SelectColor() const -> Color { return m_Displayer.SelectColor(); }
    int   Font() const { return m_Displayer.Font(); };
    float FontSize() const { return m_Displayer.FontSize(); };
    int   Padding() const { return m_Displayer.Padding(); };

protected:
    StringDisplayer& m_Displayer;
    Vec2<int> m_Mouse;

    int m_ScrollX = 0;

    void UpdateScroll()
    {
        auto pos = m_Displayer.IndexToPosition(m_Displayer.Container().Selection().start);

        if (pos.x > m_ScrollX + Width() - m_Displayer.Padding() * 2) {
            m_ScrollX = pos.x - Width() + m_Displayer.Padding() * 2;
        }
        else if (pos.x < m_ScrollX + m_Displayer.Padding()) {
            m_ScrollX = pos.x - m_Displayer.Padding();
        }
    }
};