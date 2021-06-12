#include "GuiCode/ui/graphics/Collection.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

int CommandCollection::Width(const std::string& s)
{
    return Graphics::StringWidth(s, m_Font, m_FontSize);
}

int CommandCollection::Width(const std::string_view& s)
{
    return Graphics::StringWidth(s, m_Font, m_FontSize);
}