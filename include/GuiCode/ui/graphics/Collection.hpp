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

    template<>
    void Command<Graphics::Type::PushMatrix>()
    {
        m_MatrixStack.push(m_Matrix);
        m_CommandCollection.emplace_back(Graphics::Type::PushMatrix);
    }

    template<>
    void Command<Graphics::Type::PopMatrix>()
    {
        if (m_MatrixStack.size() > 1)
        {
            m_Matrix = m_MatrixStack.top();
            m_MatrixStack.pop();
        }
        m_CommandCollection.emplace_back(Graphics::Type::PopMatrix);
    }

    template<>
    void Command<Graphics::Type::Translate, Vec2<int>>(Vec2<int> trans)
    {
        m_Matrix = glm::translate(m_Matrix, glm::vec3(trans.x, trans.y, 0));
        m_CommandCollection.emplace_back(Graphics::Type::Translate, trans);
    }

    Vec2<int> Translate() { return { (int)m_Matrix[3][0], (int)m_Matrix[3][1] }; };

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
     * Append another command collection to this one.
     * @param d command collection
     */
    void Append(CommandCollection& d)
    {
        for (auto& e : d.Get())
            m_CommandCollection.push_back(e);
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
    std::stack<glm::mat4> m_MatrixStack;
    glm::mat4 m_Matrix{ 1.0f };
};