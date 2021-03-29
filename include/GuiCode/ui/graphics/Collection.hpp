#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/graphics/Command.hpp"

/**
 * A collection of <code>Graphics::Command</code>.
 */
class CommandCollection
{
public:
    /**
     * Emplace a <code>Graphics::Command</code>.
     * @tparam T type
     * @param ...args arguments for the constructor of <code>T</code>
     */
    template<Graphics::Type T, typename ...Args>
    void Command(Args ...args)
    {
        m_CommandCollection.emplace_back(T, args...);
    }

    template<>
    void Command<Graphics::Type::Font, int, float>(int font, float size)
    {
        m_Font = font;
        m_FontSize = size;
        m_CommandCollection.emplace_back(Graphics::Type::Font, font, size);
    }

    template<>
    void Command<Graphics::Type::FontSize, float>(float size)
    {
        m_FontSize = size;
        m_CommandCollection.emplace_back(Graphics::Type::Font, m_Font, size);
    }

    int Width(const std::string& s);

    /**
     * Returns the vector of <code>Graphics::CommandBase</code>.
     * @return all commands
     */
    const std::vector<Graphics::CommandBase>& Get() const
    {
        return m_CommandCollection;
    }

    /**
     * Empties the collection.
     */
    void Clear()
    {
        m_CommandCollection.clear();
    }

private:
    int m_Font = -1;
    int m_FontSize = 48;
    std::vector<Graphics::CommandBase> m_CommandCollection;
};