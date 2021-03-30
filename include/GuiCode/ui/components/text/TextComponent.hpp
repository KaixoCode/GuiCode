#pragma once
#include <GuiCode/pch.hpp>
#include "GuiCode/ui/components/Component.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"
#include "GuiCode/ui/components/text/TextBox.hpp"

template<Align a = Align::CENTER>
class TextComponent : public TextBox
{
public:
	TextComponent(const std::string& t, Color color = { 0, 0, 0, 255 }, int f = Graphics::Fonts::Gidole14, float fsize = 14)
	{
		m_Displayer.Container().Content(t);
		Font(f, fsize);
		AlignLines(a);
		TextColor(color);
		Editable(false);
		Width(Graphics::StringWidth(t, f, fsize) + 4);
		Height(fsize + 4);
		Padding(2);
		Background({ 0, 0, 0, 0 });
	}
};
