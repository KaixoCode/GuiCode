#include "GuiCode/ui/components/LayoutManager.hpp"

// --------------------------------------------------------------------------
// --------------------------- LayoutManager --------------------------------
// --------------------------------------------------------------------------

LayoutManager::LayoutManager()
{
    m_Listener += [this](Event::MousePressed& e)
    {
        m_PMouseX = e.x;
        m_PMouseY = e.y;
        m_MouseX = e.x;
        m_MouseY = e.y;
        m_MousePress = e.button;
        //done = false;
    };

    m_Listener += [this](Event::MouseReleased& e)
    {
        m_MouseDrag = Event::MouseButton::NONE;
        m_Dragging = -1;
        //done = false;
    };

    m_Listener += [this](Event::MouseMoved& e)
    {
        m_MouseX = e.x;
        m_MouseY = e.y;
    };

    m_Listener += [this](Event::MouseDragged& e)
    {
        m_MouseX = e.x;
        m_MouseY = e.y;
        m_MouseDrag = e.button;
    };
}