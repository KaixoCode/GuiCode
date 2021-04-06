#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/panels/Panel.hpp"
#include "GuiCode/ui/components/text/TextContainer.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"
#include "GuiCode/ui/Clipboard.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Text Displayer ------------------------------
// --------------------------------------------------------------------------

/**
 * TextDisplayer displays text (wow surprising).
 */
class TextDisplayer : public Panel
{
public:

	/**
	 * Wrap enum. Word wraps at words, and at character level if a word is longer than a line. 
	 * Character wraps at any character. And None doesn't wrap at all.
	 */
	enum class Wrap
	{
		Word, Character, None
	};

	/**
	 * Constructor
	 */
	TextDisplayer();

	void Update(const Vec4<int>& v) override;
	void Render(CommandCollection& d) override;

	/**
	 * Get the TextContainer.
	 * @return TextContainer
	 */
	auto Container() -> TextContainer& { return m_Container; }

	/**
	 * Returns true if the mouse is dragging.
	 * @return true if dragging
	 */
	bool Dragging() { return m_Dragging; }

	/**
	 * Converts an (x, y) position on this TextDisplayer to an index in 
	 * the content of the TextContainer.
	 * @return index
	 */
	int PositionToIndex(const Vec2<int>& pos);

	/**
	 * Converts an index in the TextContainer to an (x, y) position
	 * on this TextDisplayer.
	 * @return position
	 */
	Vec2<int> IndexToPosition(int index);

	/**
	 * Set the horizontal alignment of the lines in this TextDisplayer.
	 * @param a alignment
	 */
	void AlignLines(Align a) { m_Align = a; }

	/**
	 * Set the content of this container.
	 * @param c content
	 */
	void Content(const std::string& c)
	{
		if (c != m_Container.Content())
		{
			m_Container.Content(c);
			RecalculateLines();
		}
	}

	/**
	 * Set if this container is editable.
	 * @param a editable
	 */
	void Editable(bool e) { m_Container.Editable(e); }

	/**
	 * Set the text color.
	 * @param c color
	 */
	void TextColor(Color c) { m_TextColor = c; }

	/**
	 * Set the selection color.
	 * @param c color
	 */
	void SelectColor(Color c) { m_SelectColor = c; }

	/**
	 * Set the text wrap mode.
	 * @param wrap wrap mode
	 */
	void TextWrap(Wrap wrap) { m_Wrap = wrap; }

	/**
	 * Set the line height.
	 * @param h line height
	 */
	void LineHeight(int h) { m_LineHeight = h; };

	/**
	 * Set the font and font size.
	 * @param f font
	 * @param size font size
	 */
	void Font(int f, float size) { m_Font = f, m_FontSize = size; };

	/**
	 * Set the padding around the text.
	 * @param p padding
	 */
	void Padding(int p) { m_Padding = p; };

	/**
	 * Get the current horizontal alignment of the text.
	 * @return text alignment
	 */
	auto AlignLines() const -> Align { return m_Align; }
	
	/**
	 * Get the entire content of this container as a string.
	 * @return content
	 */
	auto Content() -> std::string& { return m_Container.Content(); }

	/**
	 * Returns true when this text container is editable.
	 * @return true if editable
	 */
	bool Editable() const { return m_Container.Editable(); }

	/**
	 * Get the text color.
	 * @return text color
	 */
	auto TextColor() const -> Color { return m_TextColor; }

	/**
	 * Get the selection color.
	 * @return selection color
	 */
	auto SelectColor() const -> Color { return m_SelectColor; }

	/**
	 * Get the current wrap mode.
	 * @return wrap
	 */
	auto TextWrap() const -> Wrap { return m_Wrap; }

	/**
	 * Get the current line height.
	 * @return line height
	 */
	int LineHeight() const { return m_LineHeight; };

	/**
	 * Get the current font.
	 * @return font
	 */
	int Font() const { return m_Font; };

	/**
	 * Get the current font size.
	 * @return font size
	 */
	float FontSize() const { return m_FontSize; };

	/**
	 * Get the current padding around the text.
	 * @return padding 
	 */
	int Padding() const { return m_Padding; };

	/**
	 * Set a placeholder string.
	 * @param p placeholder
	 */
	void Placeholder(const std::string& p) { m_Placeholder = p; }

	/**
	 * Get the placeholder string.
	 * @return placeholder
	 */
	auto Placeholder() -> std::string& { return m_Placeholder; }

	bool Focused() const override { return Panel::Focused(); }
	void Focused(bool v) override { m_Timer = 60; Panel::Focused(v); }


private:
	TextContainer m_Container;

	std::string m_Placeholder;

	int m_LineHeight = 16,
		m_FontSize = 14,
		m_Padding = 2,
		m_Font = Graphics::Fonts::Gidole14,
		m_BiggestX = 0,
		m_TypeX = 0,
		m_Click = 0,
		m_Timer = 60;

	bool m_Shift = false,
		m_Dragging = false;

	Color m_TextColor{ 0, 0, 0, 255 }, 
		m_SelectColor{ 128, 128, 255, 255 };

	Align m_Align = Align::LEFT;

	Vec2<int> 
		m_PressPos{ 0, 0 },
		m_PrevSize{ 0, 0 };

	Wrap m_Wrap = Wrap::Word;

	std::vector<std::string_view> m_Lines;
	std::vector<int> m_LineWidths;

	void RecalculateLines();
	void CalcLinesWordWrap();
	void CalcLinesCharWrap();
	void CalcLinesNoWrap();

	void KeyTypeActions(Event::KeyTyped& e);
	void CtrlTypeActions(Event::KeyTyped& e);

	void ChangeIndexActions(Event::KeyPressed& e);

	void UpdateTypeX();
};