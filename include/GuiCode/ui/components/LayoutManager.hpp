#pragma once
#include "GuiCode/ui/components/Layout.hpp"
#include "GuiCode/ui/components/Component.hpp"
#include "GuiCode/ui/events/EventListener.hpp"

// --------------------------------------------------------------------------
// --------------------------- LayoutManager --------------------------------
// --------------------------------------------------------------------------

struct LayoutManager
{
    LayoutManager();

    void AddEvent(Event& e) { m_Listener.AddEvent(e); }

    int Cursor() const { return m_Cursor; }
    
    template<typename TypeCollection>
    void Update(const Vec4<int>& dim, TypeCollection& components) 
    {
        m_Cursor = -1;
        if (m_Layout == Layout::Grid) UpdateLayoutGrid(dim, components);
        else if (m_Layout == Layout::Stack) UpdateLayoutStack(dim, components);
        else if (m_Layout == Layout::Border) UpdateLayoutBorder(dim, components);
        m_MousePress = Event::MouseButton::NONE;
    }

    ::Layout Layout() const{ return m_Layout; }

    template<Layout::Type T, typename ...Args>
    void Layout(Args&& ...args) { m_Layout = ::Layout{ T, args... }; }

    template<typename TypeCollection>
    void UpdateLayoutGrid(const Vec4<int>& dim, TypeCollection& components)
    {
        Layout::GridLayout& _grid = m_Layout.grid;
        
        int _index = 0;
        for (auto& _c : components)
        {
            // Calculate the index in the grid
            int _xindex = _index % _grid.cols;
            int _yindex = _index / _grid.cols;
            if (_yindex > _grid.rows)
                break; // Break when more rows

            // Calculate a single slot size
            int _w = (dim.width - _grid.hgap) / _grid.cols;
            int _h = (dim.height - _grid.vgap) / _grid.rows;

            // Calculate the actual position
            int _x = _xindex * _w + _grid.hgap + dim.x;
            int _y = _yindex * _h + _grid.vgap + dim.y;

            _c->Size({ _w - _grid.hgap, _h - _grid.vgap });
            _c->Position({ _x, _y });

            _index++;
        }
    }

    template<typename TypeCollection>
    void UpdateLayoutBorder(const Vec4<int>& dim, TypeCollection& components)
    {
        Layout::BorderLayout& _border = m_Layout.border;

        int _p = _border.innerpadding;
        int _p2 = _border.outerpadding;
        bool _rN = _border.resizeN;
        bool _rE = _border.resizeE;
        bool _rS = _border.resizeS;
        bool _rW = _border.resizeW;
        Component* _north = 0;
        Component* _south = 0;
        Component* _east = 0;
        Component* _west = 0;
        Component* _center = 0;

        for (auto& _c : components) if (_c->Visible()) switch (_c->LayoutHint())
        {
        case Layout::Hint::North: _north = _c.get(); break;
        case Layout::Hint::South: _south = _c.get(); break;
        case Layout::Hint::East: _east = _c.get(); break;
        case Layout::Hint::West: _west = _c.get(); break;
        case Layout::Hint::Center: _center = _c.get(); break;
        }

        // Calculate dimensions for north if it exists
        int _northHeight = _p2;
        if (_north)
        {
            // If resizing this component
            if (m_Dragging == 1)
            {
                m_Cursor = GLFW_RESIZE_NS_CURSOR;
                _north->Height(m_Constrain(m_PDragVal - m_MouseY + m_PMouseY, _north->MinHeight(), _north->MaxHeight()));
            }

            // Dimensions
            _northHeight += _north->Height() + _p;
            int _x = _p2,
                _y = dim.height - _northHeight + _p,
                _w = dim.width - _p2 * 2,
                _h = _north->Height();

            // Detect mousepress on resize area
            if (m_MouseX > _x && m_MouseX < _x + _w &&
                m_MouseY >= _y - _p && m_MouseY <= _y && _rN)
            {
                m_Cursor = GLFW_RESIZE_NS_CURSOR;
                if (m_MousePress == Event::MouseButton::LEFT)
                {
                    m_PMouseX = m_MouseX;
                    m_PDragVal = _north->Height();
                    m_Dragging = 1;
                }
            }

            // Set dimensions
            _north->Width(_w);
            _north->Position({ _x, _y });
        }

        // Calculate dimensions for south if it exists
        int _southHeight = _p2;
        if (_south)
        {
            // If resizing this component
            if (m_Dragging == 2)
            {
                m_Cursor = GLFW_RESIZE_NS_CURSOR;
                _south->Height(m_Constrain(m_PDragVal + m_MouseY - m_PMouseY, _south->MinHeight(), _south->MaxHeight()));
            }

            // Dimensions
            _southHeight += _south->Height() + _p;
            int _x = _p2,
                _y = _p2,
                _w = dim.width - _p2 * 2,
                _h = _south->Height();

            // Detect mousepress on resize area
            if (m_MouseX > _x && m_MouseX < _x + _w &&
                m_MouseY >= _y + _h && m_MouseY <= _y + _h + _p && _rS)
            {
                m_Cursor = GLFW_RESIZE_NS_CURSOR;
                if (m_MousePress == Event::MouseButton::LEFT)
                {
                    m_PMouseX = m_MouseX;
                    m_PDragVal = _south->Height();
                    m_Dragging = 2;
                }
            }

            // Set dimensions
            _south->Width(_w);
            _south->Position({ _x, _y });
        }

        // Calculate dimensions for east if it exists
        int _eastWidth = _p2;
        if (_east)
        {
            // If resizing this component
            if (m_Dragging == 3)
            {
                m_Cursor = GLFW_RESIZE_EW_CURSOR;
                _east->Width(m_Constrain(m_PDragVal - m_MouseX + m_PMouseX, _east->MinWidth(), _east->MaxWidth()));
            }

            // Dimensions
            _eastWidth += _east->Width() + _p;
            int _x = dim.width - _eastWidth + _p2,
                _y = _southHeight,
                _w = _eastWidth - _p - _p2,
                _h = dim.height - _southHeight - _northHeight;

            // Detect mousepress on resize area
            if (m_MouseX >= _x - _p && m_MouseX <= _x &&
                m_MouseY > _y && m_MouseY < _y + _h && _rE)
            {
                m_Cursor = GLFW_RESIZE_EW_CURSOR;
                if (m_MousePress == Event::MouseButton::LEFT)
                {
                    m_PMouseX = m_MouseX;
                    m_PDragVal = _east->Width();
                    m_Dragging = 3;
                }
            }

            // Set dimensions
            _east->Size({ _w, _h });
            _east->Position({ _x, _y });
        }

        // Calculate dimensions for east if it exists
        int _westWidth = _p2;
        if (_west)
        {
            // If resizing this component
            if (m_Dragging == 4)
            {
                m_Cursor = GLFW_RESIZE_EW_CURSOR;
                _west->Width(m_Constrain(m_PDragVal + m_MouseX - m_PMouseX, _west->MinWidth(), _west->MaxWidth()));
            }

            // Dimensions
            _westWidth += _west->Width() + _p;
            int _x = _p2,
                _y = _southHeight,
                _w = _westWidth - _p - _p2,
                _h = dim.height - _southHeight - _northHeight;

            // Detect mousepress on resize area
            if (m_MouseX >= _x + _w && m_MouseX <= _x + _w + _p &&
                m_MouseY > _y && m_MouseY < _y + _h && _rW)
            {
                m_Cursor = GLFW_RESIZE_EW_CURSOR;
                if (m_MousePress == Event::MouseButton::LEFT)
                {
                    m_PMouseX = m_MouseX;
                    m_PDragVal = _west->Width();
                    m_Dragging = 4;
                }
            }

            // Set dimensions
            _west->Size({ _w, _h });
            _west->Position({ _x, _y });
        }

        // Dimensions for the center one, depends on all other components
        if (_center)
        {
            int _w = dim.width - _westWidth - _eastWidth;
            int _h = dim.height - _southHeight - _northHeight;

            // If not wide enough, check if we can steal some area from other components
            if (_w < _center->MinWidth())
            {
                // The necessary difference
                int _diff = _center->MinWidth() - _w;
                int _diffo = 0;
                bool _done = false;

                // Check the west component for some space, if it is resizable
                if (_rW && _west && (_diffo = _west->Width() - _west->MinWidth()) > 0)
                {
                    // Is the available space enough?
                    int _enough = _diff - _diffo;
                    if (_enough <= 0)
                        _done = true;

                    // Adjust the width of both the center and the west.
                    int _pw = _w;
                    _w = min(_w + _diffo, _w + _diff);
                    _west->Width(_west->Width() - (_w - _pw));
                    _westWidth -= (_w - _pw);
                }

                // If east is resizable and resizing is not done yet
                if (_rE && !_done && _east && (_diffo = _east->Width() - _east->MinWidth()) > 0)
                {
                    // Adjust the widths
                    int _pw = _w;
                    _w = min(_w + _diffo, _w + _diff);
                    _east->Width(_east->Width() - (_w - _pw));
                }
            }

            // Now check if the height is big enough
            if (_h < _center->MinHeight())
            {
                // Calculate the necessary difference
                int _diff = _center->MinHeight() - _h;
                int _diffo = 0;
                bool _done = false;

                // Check north component if it's resizable
                if (_rN && _north && (_diffo = _north->Height() - _north->MinHeight()) > 0)
                {
                    // Is it enough?
                    int _enough = _diff - _diffo;
                    if (_enough <= 0)
                        _done = true;

                    // Adjust the heights
                    int _ph = _h;
                    _h = min(_h + _diffo, _h + _diff);
                    _north->Height(_north->Height() - (_h - _ph));
                }

                // If it is not done yet and south is resizable
                if (_rS && !_done && _south && (_diffo = _south->Height() - _south->MinHeight()) > 0)
                {
                    // Adjust the heights
                    int _ph = _h;
                    _h = min(_h + _diffo, _h + _diff);
                    _south->Height(_south->Height() - (_h - _ph));
                    _southHeight -= (_h - _ph);
                    
                    // Also resize the east and west since they depend on the height of south
                    if (_east)
                    {
                        _east->Position({ _east->X(), _southHeight });
                        _east->Height(_east->Height() + (_h - _ph));
                    }
                    
                    if (_west)
                    {
                        _west->Position({ _west->X(), _southHeight });
                        _west->Height(_west->Height() + (_h - _ph));
                    }
                }
            }
            _center->Size({ _w, _h });
            _center->Position({ _westWidth, _southHeight });
        }
    }

    template<typename TypeCollection>
    void UpdateLayoutStack(const Vec4<int>& dim, TypeCollection& components)
    {
        Layout::StackLayout& _stack = m_Layout.stack;

        int _padding = _stack.padding;
        int _y = dim.y;
        size_t _size = components.size();
        for (int _i = _size - 1; _i >= 0; _i--)
        {
            auto& _c = components[_i];
            if (!_c->Visible())
                continue;

            int _x = dim.x;
            _c->Position({ _x, _y });
            _c->Width(dim.width);
            _y += _c->Height() + _padding;
        }
        m_BiggestX = dim.x + dim.width;
        m_BiggestY = _y;
    }

    Vec2<int> BiggestCoords() { return { m_BiggestX, m_BiggestY }; }

private:

    int m_Constrain(int a, int b, int c)
    {
        if (c == -1)
            return a < b ? b : a;
        else
            return a < b ? b : a > c ? c : a;
    }


    ::Layout m_Layout = { ::Layout::Free };
    EventListener m_Listener;

    int m_MouseX = 0,
        m_MouseY = 0,
        m_PMouseX = 0,
        m_PMouseY = 0,
        m_PDragVal = 0,
        m_Dragging = 0,
        m_Cursor = -1,
        m_BiggestX = 0,
        m_BiggestY = 0;

    Event::MouseButton m_MouseDrag = Event::MouseButton::NONE,
        m_MousePress = Event::MouseButton::NONE;
};