#pragma once
#include "GuiCode/pch.hpp"

class Selection : public Vec2<int>
{
public:
	int Lowest() const { return start < end ? start : end; }
	int Highest() const { return start < end ? end : start; }
};

class StringContainer
{
public:

	StringContainer(const std::string& c = "", bool editable = true)
		: m_Content(c), m_Editable(editable)
	{}

	bool Editable() const { return m_Editable; }
	void Editable(bool e) { m_Editable = e; }
	int  Length() const { return m_Content.length(); }
	auto Selection() -> Selection& { return m_Selection; }
	bool Selected() { return m_Selection.start != m_Selection.end; }
	void Select(const ::Selection& s) { m_Selection = s; ConstrainSelection(); }
	void Select(int pos) { m_Selection = { pos, pos }; ConstrainSelection(); }
	auto Content() -> std::string& { return m_Content; }
	void Content(const std::string& c) { m_Content = c; }

	void Insert(const std::string& add) 
	{ 
		RemoveSelection();
		auto& _start = m_Content.begin() + Selection().Lowest();
		m_Content.insert(_start, add.begin(), add.end());
		Select(Selection().Lowest() + add.length());
	}

	void Insert(const char& add)
	{
		RemoveSelection();
		auto& _start = m_Content.begin() + Selection().Lowest();
		m_Content.insert(_start, add);
		Select(Selection().Lowest() + 1);
	}

	void Delete()
	{
		if (Selected())
			RemoveSelection();

		else if (Selection().Highest() < Length())
		{
			auto& _start = m_Content.begin() + Selection().Lowest();
			m_Content.erase(_start, _start + 1);
			Select(Selection().Lowest());
		}
	}

	void Backspace()
	{
		if (Selected())
			RemoveSelection();

		else if (Selection().Lowest() > 0)
		{
			auto& _start = m_Content.begin() + Selection().Lowest();
			m_Content.erase(_start - 1, _start);
			Select(Selection().Lowest() - 1);
		}
	}
	
	void RemoveSelection()
	{
		Remove(Selection());
		Select(Selection().Lowest());
	}

	void Remove(const ::Selection& s)
	{
		auto& _start = m_Content.begin() + s.Lowest();
		auto& _end = m_Content.begin() + s.Highest();
		m_Content.erase(_start, _end);
		ConstrainSelection();
	}

	int CtrlLeft() 
	{
		return CtrlLeft(Selection().start);
	}

	int CtrlLeft(int start)
	{
		if (start > Length())
			return Length();

		if (start < 0)
			return 0;

		// Get the text up until the type index
		std::string_view text = Content();
		text = text.substr(0, start);

		// If there is no text, keep the same
		if (text.length() == 0)
			return start;

		// Get the type of character at the end of the string
		bool letter = text[text.length() - 1] < 0 ? false : std::isalpha(text[text.length() - 1]);

		// Loop through each index from back to front
		for (int index = start - 1; index >= 0; index--)
		{
			// If there's a change in character type, return the index + 1, because we want
			// the index after the change in character type.
			bool alpha = text[index] < 0 ? false : std::isalpha(text[index]);
			if (letter != alpha)
				return index + 1;
		}
	
		// Otherwise if it went through the entire loop, it's at the start of the text,
		// so index will be 0.
		return 0;
	}

	int CtrlRight() 
	{
		return CtrlRight(Selection().start);
	}

	int CtrlRight(int start)
	{
		if (start > Length())
			return Length();

		if (start < 0)
			return 0;

		// Get the text from index to end
		std::string_view text = Content();
		text = text.substr(start);

		// If there is no text, keep the same
		if (text.length() == 0)
			return start;

		// Get the type of character at the start of the string
		bool letter = text[0] < 0 ? false : std::isalpha(text[0]);

		// Loop through each index from front to back
		for (int index = 0; index < text.length(); index++)
		{
			// If there's a change in character type, return the index - 1 + start, because
			// we want the index after the change in character type.
			bool alpha = text[index] < 0 ? false : std::isalpha(text[index]);
			if (letter != alpha)
				return index + start;
		}
		// Otherwise if it went through the entire loop, it's at the end of the text,
		// so index will be length.
		return Length();
	}

private:
	std::string m_Content;

	bool m_Editable;

	::Selection m_Selection{ 0, 0 };

	void ConstrainSelection() { m_Selection = { constrain(m_Selection.start, 0, Length()), constrain(m_Selection.end, 0, Length()) }; }
};