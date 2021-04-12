#pragma once
#include "GuiCode/pch.hpp"

// --------------------------------------------------------------------------
// ------------------------------ Selection ---------------------------------
// --------------------------------------------------------------------------

/**
 * Simple Selection class, practically just a <code>Vec2<int></code> but with a <code>Lowest()</code> 
 * and <code>Highest()</code> method to retrieve the lowest/highest index.
 */
class Selection : public Vec2<int>
{
public:

	/**
	 * Get the lowest value.
	 * @return lowest value
	 */
	int Lowest() const { return start < end ? start : end; }

	/**
	 * Get the highest value.
	 * @return highest value
	 */
	int Highest() const { return start < end ? end : start; }
};

// --------------------------------------------------------------------------
// ---------------------------- Text Container ------------------------------
// --------------------------------------------------------------------------

/**
 * Contains text, used in TextDisplayer. Tracks typing location and text selection.
 */
class TextContainer
{
public:

	/**
	 * Constructor
	 * @param c content
	 * @param editable editable
	 */
	TextContainer(const std::string& c = "", bool editable = true);

	/**
	 * Returns true when this text container is editable.
	 * @return true if editable
	 */
	bool Editable() const { return m_Editable; }

	/**
	 * Set if this container is editable.
	 * @param a editable
	 */
	void Editable(bool e) { m_Editable = e; }

	/**
	 * Get the length of the content in this container.
	 * @return length
	 */
	int Length() const { return m_Content.length(); }

	/**
	 * Get the selection as indices.
	 * @return selection
	 */
	auto Selection() -> Selection& { return m_Selection; }

	/**
	 * Get the selection as a <code>std::string_view</code>.
	 * @return selection
	 */
	auto SelectionString() -> std::string_view { return std::string_view(m_Content).substr(m_Selection.Lowest(), (size_t)m_Selection.Highest() - m_Selection.Lowest()); }
	
	/**
	 * Returns true when there is text selected (selection.start != selection.end).
	 * @return true if selection
	 */
	bool Selected() const { return m_Selection.start != m_Selection.end; }

	/**
	 * Set a selection. This selection is automatically constrained to (0, Length()).
	 * @param s selection
	 */
	void Select(const ::Selection& s) { m_Selection = s; ConstrainSelection(); }

	/**
	 * Set a selection. This selection is automatically constrained to (0, Length()).
	 * @param pos selection
	 */
	void Select(int pos) { m_Selection = { pos, pos }; ConstrainSelection(); }
	
	/**
	 * Get the entire content of this container as a string.
	 * @return content
	 */
	auto Content() -> std::string& { return m_Content; }

	/**
	 * Set the content of this container.
	 * @param c content
	 */
	void Content(const std::string& c) { m_Content = c; }

	/**
	 * Insert a string at the current selection, removes the selection (if there is one)
	 * and then inserts the string in its place.
	 * @param add insertion
	 */
	void Insert(const std::string& add);

	/**
	 * Insert a characer at the current selection, removes the selection (if there is one)
	 * and then inserts the character in its place.
	 * @param add insertion
	 */
	void Insert(const char& add);

	/**
	 * Normal delete operation, if there is a selection it gets removed, otherwise the
	 * character that comes after the current typing index gets removed.
	 */
	void Delete();

	/**
	 * Backspace operation, if there is a selection it gets removed, otherwise the
	 * character before the current typing index gets removed.
	 */
	void Backspace();
	
	/**
	 * Removes the selection from the content. Automatically adjusts the typing index.
	 */
	void RemoveSelection();

	/**
	 * Removes the given selection from the content. Does not adjust the typing index.
	 * @param s selection
	 */
	void Remove(const ::Selection& s);

	/**
	 * Get the index in the content 1 word to the left of the current typing index.
	 * @return index
	 */
	int CtrlLeft();

	/**
	 * Get the index in the content 1 word to the left of the given index.
	 * @param start start index
	 * @return index
	 */
	int CtrlLeft(int start);
	
	/**
	 * Get the index in the content 1 word to the right of the current typing index.
	 * @return index
	 */
	int CtrlRight();

	/**
	 * Get the index in the content 1 word to the right of the given index.
	 * @param start start index
	 * @return index
	 */
	int CtrlRight(int start);

	void Filter(const std::string& r) { m_EnableRegex = true, m_Filter = r; }
	void Filter(const std::regex& r) { m_EnableRegex = true, m_Filter = r; }
	auto Filter() -> std::regex& { return m_Filter; }
	void EnableFilter(bool b) { m_EnableRegex = b; }
	bool FilterEnabled() { return m_EnableRegex; }

private:
	std::string m_Content;
	std::regex m_Filter{ ".*", std::regex_constants::ECMAScript };

	bool m_Editable = true,
		m_EnableRegex = false;

	::Selection m_Selection{ 0, 0 };

	void ConstrainSelection() { m_Selection = { constrain(m_Selection.start, 0, Length()), constrain(m_Selection.end, 0, Length()) }; }
};