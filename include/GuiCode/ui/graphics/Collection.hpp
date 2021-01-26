#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/graphics/Command.hpp"

class CommandCollection
{
public:
    template<Graphics::Type T, typename ...Args>
    void Command(Args ...args)
    {
       // m_CommandCollection.emplace_back(std::make_unique<Graphics::Command<T>>(std::forward<Args>(args)...));
        m_CommandCollection.emplace_back(T, args...);
    }

    const std::vector<Graphics::CommandBase>& Get() const
    {
        return m_CommandCollection;
    }

    void Clear()
    {
        m_CommandCollection.clear();
    }

private:
    std::vector<Graphics::CommandBase> m_CommandCollection;
};