#include "GuiCode/ui/components/text/TextDisplayer.hpp"

// --------------------------------------------------------------------------
// ---------------------------- Text Displayer ------------------------------
// --------------------------------------------------------------------------

TextDisplayer::TextDisplayer()
{
	m_Listener += [this](Event::KeyReleased& e)
	{
		if (e.key == Key::SHIFT)
			m_Shift = false;
	};

	m_Listener += [this](Event::KeyPressed& e)
	{
		if (!m_Focused)
			return;

		m_Timer = 60;
			
		if (e.key == Key::SHIFT)
			m_Shift = true;

		if (e.key == Key::DEL)
			m_Container.Delete();

		if (e.key == Key::LEFT || e.key == Key::RIGHT || e.key == Key::UP || e.key == Key::DOWN)
			ChangeIndexActions(e);
		RecalculateLines();
	};

	m_Listener += [this](Event::KeyTyped& e)
	{
		if (!m_Focused)
			return;

		m_Timer = 60;

		if (e.keymod & Event::Mod::CONTROL)
			CtrlTypeActions(e);
		else if (m_Container.Editable())
			KeyTypeActions(e);
		RecalculateLines();
	};

	m_Listener += [this](Event::MouseClicked& e)
	{
		if (e.button != Event::MouseButton::LEFT || m_Shift)
			return;

		if (m_Click > 0 && m_PressPos == Vec2<int>{ e.x, e.y })
			m_Container.Select({ m_Container.CtrlLeft(), m_Container.CtrlRight() });

		m_PressPos = { e.x, e.y };

		m_Click = 20;
	};

	m_Listener += [this](Event::MousePressed& e)
	{
		if (e.button != Event::MouseButton::LEFT)
			return;

		m_Timer = 60;

		if (m_Shift)
			m_Container.Select({ PositionToIndex({ e.x, e.y }), m_Container.Selection().end });
		else
			m_Container.Select(PositionToIndex({ e.x, e.y }));

		UpdateTypeX();

		m_Dragging = true;
	};

	m_Listener += [this](Event::MouseReleased& e)
	{
		RecalculateLines();
		if (e.button != Event::MouseButton::LEFT)
			return;

		m_Dragging = false;
	};

	m_Listener += [this](Event::MouseDragged& e)
	{
		if (e.button != Event::MouseButton::LEFT)
			return;

		if (m_Dragging)
		{
			m_Container.Select({ PositionToIndex({ e.x, e.y }), m_Container.Selection().end });
			UpdateTypeX();
		}
	};

	m_Listener += [this](Event::Unfocused& e)
	{
		m_Container.Select(m_Container.Selection().start);
		m_Focused = false;
	};
	m_Listener += [this](Event::Focused& e)
	{
		RecalculateLines();
		m_Focused = true;
	};

	m_Cursor = GLFW_IBEAM_CURSOR;

	RecalculateLines();
}

void TextDisplayer::Update(const Vec4<int>& v)
{
	m_Timer--;
	if (!m_Timer)
		m_Timer = 60;

	if (m_Click)
		m_Click--;

	Component::Update(v);
	Height(m_Lines.size() * m_LineHeight + 2 * m_Padding);


	if (m_PrevSize != m_Size)
	{
		RecalculateLines();
		m_PrevSize = m_Size;
	}

	if (m_Wrap == Wrap::None)
		Width(std::max(m_BiggestX, m_MinSize.width));
}
	
void TextDisplayer::Render(CommandCollection& d)
{
	Component::Render(d);
	d.Command<Graphics::TextAlign>(Vec2<Align>{Align::LEFT, Align::CENTER});
	d.Command<Graphics::Font>(m_Font, m_FontSize);
	int padding = m_Padding;
	int lineheight = m_LineHeight;
	int y = Height() - padding - lineheight;
	int beginindex = 0;
	auto sel = m_Container.Selection();
	d.Command<Graphics::Fill>(m_TextColor);
	int index = 0;
	if (m_Lines.size() == 1 && m_Lines[0].length() == 0 && !m_Focused)
	{
		int nx = 0;
		if (m_Align == Align::CENTER)
			nx = Width() / 2 - m_LineWidths[index] / 2;
		else if (m_Align == Align::RIGHT)
			nx = Width() - m_LineWidths[index];

		d.Command<Graphics::Fill>(Color{ m_TextColor.r, m_TextColor.g, m_TextColor.b, (float)(m_TextColor.a * 0.5) });
		d.Command<Graphics::Text>(&m_Placeholder, Vec2<int>{ X() + padding + nx, Y() + y + lineheight / 2 });
	}

	for (auto& i : m_Lines)
	{
		if (y + Y() + lineheight < m_Viewport.y || y + Y() > m_Viewport.y + m_Viewport.height)
		{
			y -= lineheight;
			beginindex += i.length();
			index++;
			continue;
		}
		bool low = sel.Lowest() <= beginindex + i.length() && sel.Lowest() > beginindex - 1;
		bool high = sel.Highest() <= beginindex + i.length() && sel.Highest() > beginindex - 1;
		bool between = beginindex > sel.Lowest() && beginindex + i.length() < sel.Highest();
		bool pos = m_Focused && m_Timer > 30 && m_Container.Editable() &&
			(sel.start < beginindex + i.length() && sel.start > beginindex -1 || 
				i == *(m_Lines.end() - 1) && sel.start == i.length() + beginindex);

		int nx = 0;
		if (m_Align == Align::CENTER)
			nx = Width() / 2 - m_LineWidths[index] / 2;
		else if (m_Align == Align::RIGHT)
			nx = Width() - m_LineWidths[index];

		/* Selection highlight */
		if (low && !high)
		{
			std::string_view sub1 = i.substr(0, sel.Lowest() - beginindex);
			std::string_view sub2 = i.substr(sel.Lowest() - beginindex);
			int w1 = Graphics::StringWidth(sub1, m_Font, m_FontSize);
			int w2 = Graphics::StringWidth(sub2, m_Font, m_FontSize);
			d.Command<Graphics::Fill>(m_SelectColor);
			d.Command<Graphics::Quad>(Vec4<int>{ X() + padding + w1 + nx, Y() + y, w2, lineheight });
			d.Command<Graphics::Fill>(m_TextColor);
		}

		if (low && high)
		{
			std::string_view sub1 = i.substr(0, sel.Lowest() - beginindex);
			std::string_view sub2 = i.substr(0, sel.Highest() - beginindex);
			int w1 = Graphics::StringWidth(sub1, m_Font, m_FontSize);
			int w2 = Graphics::StringWidth(sub2, m_Font, m_FontSize);
			d.Command<Graphics::Fill>(m_SelectColor);
			d.Command<Graphics::Quad>(Vec4<int>{ X() + padding + w1 + nx, Y() + y, w2 - w1, lineheight });
			d.Command<Graphics::Fill>(m_TextColor);
		}

		if (between)
		{
			int w1 = Graphics::StringWidth(i, m_Font, m_FontSize);
			d.Command<Graphics::Fill>(m_SelectColor);
			d.Command<Graphics::Quad>(Vec4<int>{ X() + padding + nx, Y() + y, w1, lineheight });
			d.Command<Graphics::Fill>(m_TextColor);
		}

		if (!low && high)
		{
			std::string_view sub1 = i.substr(0, sel.Highest() - beginindex);
			int w1 = Graphics::StringWidth(sub1, m_Font, m_FontSize);
			d.Command<Graphics::Fill>(m_SelectColor);
			d.Command<Graphics::Quad>(Vec4<int>{ X() + padding + nx, Y() + y, w1, lineheight });
			d.Command<Graphics::Fill>(m_TextColor);
		}

		if (pos)
		{
			std::string_view sub1 = i.substr(0, sel.start - beginindex);
			int w1 = Graphics::StringWidth(sub1, m_Font, m_FontSize);
			d.Command<Graphics::Fill>(m_TextColor);
			d.Command<Graphics::Quad>(Vec4<int>{ X() + padding + nx + w1, Y() + y, 1, lineheight });
		}

		d.Command<Graphics::TextView>(&i, Vec2<int>{ X() + padding + nx, Y() + y + lineheight / 2 });
		y -= lineheight;
		beginindex += i.length();
		index++;
	}
}


int TextDisplayer::PositionToIndex(const Vec2<int>& pos)
{
	// Get the line index by dividing the y position minus the padding by the text
	// height. Also contrain the index to make sure no IndexOutOfBounds is thrown
	int textheight = m_LineHeight;
	int line = ((Height() - m_Padding) - pos.y) / (textheight);
	line = constrain(line, 0, m_Lines.size() - 1);

	// Increment the index up to the found line, do +1 because we split it on "\n"
	// so that isn't part of the lines anymore.
	int index = 0;
	for (int i = 0; i < line; i++)
		index += m_Lines[i].length();

	// Calculate the horizontal offset given the text alignment
	int nx = 0;
	if (m_Align == Align::CENTER)
		nx = Width() / 2 - m_LineWidths[line] / 2;
	else if (m_Align == Align::RIGHT)
		nx = Width() - m_LineWidths[line];
	
	// Now calculate the index in the line by checking each character's x-position
	// until we passes the given x-position.
	auto thisLine = m_Lines[line];
	for (int i = 0; i < thisLine.length(); i++) 
	{

		// Width of the current character
		int w = Graphics::CharWidth(thisLine[i], m_Font, m_FontSize);

		// If the current width of the line + half the width of the current character +
		// paddingX is bigger than the x, we've found our index.
		if (nx + w / 2 + m_Padding > pos.x) 
			return index + i;

		// Increament the width of this line by the character width
		nx += w;
	}

	// If the given x-position is even bigger, it means it's after the end of the
	// line, so add the entire length (-1 because of the '\n', except on last line )
	// of the line and return.
	if (line == m_Lines.size() - 1)
		return index + m_Lines[line].length();
	else
		return index + m_Lines[line].length() - 1;
}

Vec2<int> TextDisplayer::IndexToPosition(int index)
{
	// Get the font metrics of the Graphics2D
	int textheight = m_LineHeight;

	int line = 0;
	int in = index;
	while (in >= 0 && line != m_Lines.size())
		in -= m_Lines[line].length(), line++;

	line--;
	in += m_Lines[line].length();

	// Calculate the position using the index and the font metrics
	int ex = m_Padding + Graphics::StringWidth(m_Lines[line].substr(0, in), m_Font, m_FontSize);
	int ey = m_Padding + line * textheight;
	if (m_Align == Align::CENTER)
		ex += Width() / 2 - m_LineWidths[line] / 2;
	else if (m_Align == Align::RIGHT)
		ex += Width() - m_LineWidths[line];
	return { ex, Height() - ey };
}

void TextDisplayer::RecalculateLines()
{
	switch(m_Wrap)
	{
	case Wrap::None: CalcLinesNoWrap(); break;
	case Wrap::Character: CalcLinesNoWrap(); CalcLinesCharWrap(); break;
	case Wrap::Word: CalcLinesNoWrap(); CalcLinesWordWrap(); break;
	}
}

void TextDisplayer::CalcLinesWordWrap()
{
	// Big magic, good luck.

	int size = m_Lines.size();
	int index = 0;
	for (int i = 0; i < size; i++)
	{
		int w = 0;
		int lineindex = index;
		int camt = m_Lines[i].size();
		while (index - lineindex < camt)
		{
			int ctrl = m_Container.CtrlRight(index);
			int j = index - lineindex;
			int len = ctrl - lineindex - j;

			if (j + len > m_Lines[i].length() || j < 0 || j > m_Lines[i].length())
				break;

			int width = Graphics::StringWidth(m_Lines[i].substr(j, len), m_Font, m_FontSize);
			if (width > Width() - 2 * m_Padding)
			{
				int b = j;
				for (int k = j; k < camt; k++)
				{
					int nw = Graphics::CharWidth(m_Lines[i][k], m_Font, m_FontSize);
					w += nw;
					if (w > Width() - 2 * m_Padding)
					{
						auto left = m_Lines[i].substr(0, k);
						auto right = m_Lines[i].substr(k);
						m_Lines[i] = left;
						m_Lines.emplace(m_Lines.begin() + i + 1, right);
						m_LineWidths[i] = w - nw;
						m_LineWidths.emplace(m_LineWidths.begin() + i + 1, Graphics::StringWidth(m_Lines[i + 1], m_Font, m_FontSize));
						size++;
						b = k;
						break;
					}
				}
				if (index == b)
					index++;
				else
					index = b;

				break;
			}
				
			w += width;
			if (w > Width() - 2 * m_Padding)
			{
				auto left = m_Lines[i].substr(0, j);
				auto right = m_Lines[i].substr(j);
				m_Lines[i] = left;
				m_Lines.emplace(m_Lines.begin() + i + 1, right);
				m_LineWidths[i] = w - width;
				m_LineWidths.emplace(m_LineWidths.begin() + i + 1, Graphics::StringWidth(m_Lines[i + 1], m_Font, m_FontSize));
				size++;
				break;
			}

			if (index == ctrl)
				index++;
			else
				index = ctrl;
		}
	}
}

void TextDisplayer::CalcLinesCharWrap()
{
	int size = m_Lines.size();
	for (int i = 0; i < size; i++)
	{
		int w = 0;
		int camt = m_Lines[i].size();
		for (int j = 0; j < camt; j++)
		{
			int nw = Graphics::CharWidth(m_Lines[i][j], m_Font, m_FontSize);
			w += nw;
			if (w > Width() - 2 * m_Padding)
			{
				auto left = m_Lines[i].substr(0, j);
				auto right = m_Lines[i].substr(j);
				m_Lines[i] = left;
				m_Lines.emplace(m_Lines.begin() + i + 1, right);
				m_LineWidths[i] = w - nw;
				m_LineWidths.emplace(m_LineWidths.begin() + i + 1, Graphics::StringWidth(m_Lines[i + 1], m_Font, m_FontSize));
				size++;
				break;
			}
		}
	}
}

void TextDisplayer::CalcLinesNoWrap()
{
	m_BiggestX = 0;
	m_Lines.clear();
	m_LineWidths.clear();
	std::string_view str = m_Container.Content();
	std::string delims = "\n";
	for (auto first = str.data(), second = str.data(), last = first + str.size(); second != last && first != last; first = second + 1) {
		second = std::find_first_of(first, last, delims.begin(), delims.end());

		std::string_view* now;
		if (first != second)
			if (*(second) == '\n')
				now = &m_Lines.emplace_back(first, second - first + 1);
			else
				now = &m_Lines.emplace_back(first, second - first);
		else
			if (*(second) == '\n')
				now = &m_Lines.emplace_back("\n");
			else
				now = &m_Lines.emplace_back("");

		int w = Graphics::StringWidth(*now, m_Font, m_FontSize) + m_Padding * 4;
		m_LineWidths.push_back(w);
		if (w > m_BiggestX)
			m_BiggestX = w;
	}

	if (m_Lines.size() == 0)
	{
		m_Lines.emplace_back(str),
			m_BiggestX = m_Padding * 4;
		m_LineWidths.push_back(m_Padding * 4);
	}
	if (str.size() > 0 && *(str.end() - 1) == '\n')
	{
		m_Lines.emplace_back("\n");
		m_LineWidths.push_back(0);
	}
}

void TextDisplayer::KeyTypeActions(Event::KeyTyped& e)
{
	switch (e.key)
	{
	case Key::BACKSPACE: m_Container.Backspace(); break;
	case Key::TAB: m_Container.Insert("    "); break;
	case '\r': m_Container.Insert("\n"); break;
	default: m_Container.Insert(e.key);
	}
}

void TextDisplayer::CtrlTypeActions(Event::KeyTyped& e)
{
	char key = e.key;
	if (key == 127 && m_Container.Editable())
	{
		if (m_Container.Selected())
			m_Container.RemoveSelection();
		else
		{
			int ctrl = m_Container.CtrlLeft();
			m_Container.Remove({ ctrl, m_Container.Selection().start });
			m_Container.Select(ctrl);
		}
	}
	else if (key == 1)
	{
		m_Container.Select({ m_Container.Length(), 0 });
	}
	else if ((int)key == 3)
	{
		Clipboard::Get().Copy(std::string(m_Container.SelectionString()));
	}
	else if ((int)key == 24 && m_Container.Editable()) 
	{
		Clipboard::Get().Copy(std::string(m_Container.SelectionString()));
			
		m_Container.RemoveSelection();
	}
	else if ((int)key == 22 && m_Container.Editable()) 
	{
		m_Container.Insert(Clipboard::Get().Paste());			
	}

}

void TextDisplayer::ChangeIndexActions(Event::KeyPressed& e)
{
	int index = m_Container.Selection().start;

	// If theres a selection and shift is not pressed, go to the end of start of the
	// selection depending on the key.
	if (m_Container.Selected() && !(e.keymod & Event::Mod::SHIFT))
	{
		if (e.key == Key::LEFT)
			m_Container.Select(m_Container.Selection().Lowest());
		else if (e.key == Key::RIGHT)
			m_Container.Select(m_Container.Selection().Highest());
		else if (e.key == Key::UP)
			m_Container.Select(PositionToIndex({ m_TypeX, m_LineHeight + IndexToPosition(m_Container.Selection().Lowest()).y }));
		else if (e.key == Key::DOWN)
			m_Container.Select(PositionToIndex({ m_TypeX, -m_LineHeight + IndexToPosition(m_Container.Selection().Highest()).y }));
		return;
	}

	// If ctrl is down move the index an entire 'word'
	if (e.keymod & Event::Mod::CONTROL)
	{
		if (e.key == Key::LEFT)
			index = m_Container.CtrlLeft();
		else if (e.key == Key::RIGHT)
			index = m_Container.CtrlRight();
		else if (e.key == Key::UP)
			index = PositionToIndex({ m_TypeX, m_LineHeight + IndexToPosition(m_Container.Selection().start).y });
		else if (e.key == Key::DOWN)
			index = PositionToIndex({ m_TypeX, -m_LineHeight + IndexToPosition(m_Container.Selection().start).y });
	}

	// Otherwise just normal index adjusting
	else
	{
		if (e.key == Key::LEFT)
			index--;
		else if (e.key == Key::RIGHT)
			index++;
		else if (e.key == Key::UP)
			index = PositionToIndex({ m_TypeX, m_LineHeight + IndexToPosition(m_Container.Selection().start).y });
		else if (e.key == Key::DOWN)
			index = PositionToIndex({ m_TypeX, -m_LineHeight + IndexToPosition(m_Container.Selection().start).y });
	}

	if (!(e.keymod & Event::Mod::SHIFT))
		m_Container.Select(index);
	else
		m_Container.Select({ index, m_Container.Selection().end });

	if (e.key == Key::LEFT || e.key == Key::RIGHT)
		UpdateTypeX();
}

void TextDisplayer::UpdateTypeX()
{
	m_TypeX = IndexToPosition(m_Container.Selection().start).x;
}