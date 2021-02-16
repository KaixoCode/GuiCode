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
       // m_CommandCollection.emplace_back(std::make_unique<Graphics::Command<T>>(std::forward<Args>(args)...));
        m_CommandCollection.emplace_back(T, args...);
    }

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
    std::vector<Graphics::CommandBase> m_CommandCollection;
};