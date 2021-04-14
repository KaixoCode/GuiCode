#pragma once
#include "GuiCode/ui/components/Layout.hpp"
#include "GuiCode/ui/components/Component.hpp"
#include "GuiCode/ui/events/EventListener.hpp"

struct Div;
struct Div : public Component
{
    /**
     * When DivSize is set to AUTO (-1), the layout interpretter will equally divide
     * available space between all Divs marked AUTO within a Div.
     */
    static inline int AUTO = -1;

    /**
     * Allignment of objects inside of a Div.
     */
    enum class Alignment
    {
        Left, Right, Top, Bottom, Center, // For an Object only
        Vertical, Horizontal			  // For Divs only.
    };

    struct Settings
    {
        int divs;
        Alignment align = Alignment::Horizontal;
        int padding = 0;
        bool dividers = false;
        int size = AUTO;
    };

    struct ObjectSettings 
    {
        Component& component;
        Alignment align = Alignment::Center;
        int size = AUTO;
        bool resize = false;
    };

    /**
     * Type of Div. Either containing a single Object or more Divs.
     */
    enum class Type
    {
        Divs, Object
    };

    /**
     * Constructor.
     * @param t alignment
     */
    Div(Alignment t = Alignment::Horizontal)
        : m_Align(t)
    {}

    /**
     * Get the type of this Div, either Divs or Object.
     */
    auto DivType() -> Type { return m_Type; }

    /**
     * Depending on the Div type: Set the alignment of the Divs contained in this Div or the alignment of the Object in this Div.
     * @param a alignment
     */
    void Align(Alignment a) { m_Align = a; }

    /**
     * Get the current alignment of this Div.
     */
    auto Align() -> Alignment { return m_Align; }

    /**
     * Get the object if this Div is of type Object.
     * @return object
     */
    auto Object() -> Component& { return *m_Object; }

    /**
     * Set the object if this Div is of type Object.
     * @param o object
     */
    void Object(Component* o)
    {
        if (m_Align == Alignment::Vertical || m_Align == Alignment::Horizontal)
            m_Align = Alignment::Center;
        m_Type = Type::Object; m_Object = o;
    }

    /**
     * Set the object if this Div is of type Object.
     * @param o object
     */
    void Object(Component& o)
    {
        if (m_Align == Alignment::Vertical || m_Align == Alignment::Horizontal)
            m_Align = Alignment::Center;
        m_Type = Type::Object; m_Object = &o;
    }

    /**
     * Set padding for this Div.
     * @param s padding
     */
    void Padding(int s) { m_Padding = s; }

    /**
     * Get padding for this Div.
     * @return padding
     */
    int Padding() { return m_Padding; }

    /**
     * When Dividers is set to true, the interpretter will put dividers between the Divs
     * contained in this Div.
     * @param s dividers
     */
    void Dividers(bool s) { m_Dividers = s; }

    /**
     * Returns true when dividers should be displayed between the Divs inside this Div.
     * @returns true if has dividers
     */
    bool Dividers() { return m_Dividers; }

    /**
     * Enable the autoresizing of the component to the div size.
     * @param r resize
     */
    void ResizeComponent(bool r) { m_ResizeComponent = r; }

    /**
     * Returns true when auto resizing is enabled.
     * @return if auto resize
     */
    bool ResizeComponent() { return m_ResizeComponent; }

    /**
     * Set the size of this Div in pixels.
     * @param s size
     */
    void DivSize(int s) { m_CellSize = s; }

    /**
     * Get the size of this Div in pixels.
     * @return size
     */
    int  DivSize() { return m_CellSize; }

    /**
     * Returns all the Divs inside this Div.
     * @return all the Divs in this Div
     */
    auto Divs() ->std::vector<std::unique_ptr<Div>>& { return m_Divs; }

    /**
     * Set the amount of Divs in this Div.
     * @param i amount of Divs
     */
    void Divs(int i)
    {
        m_Cells = i;
        m_Type = Type::Divs;
        while (m_Divs.size() < i)
            m_Divs.emplace_back(std::make_unique<Div>());
    }

    /**
     * Get the Div at the index.
     * @param index index
     */
    Div& operator[](int index) { return *m_Divs[index]; }

    /**
     * Set the object if this Div is of type Object.
     * @param o object
     */
    Div& operator=(Component& o) { Object(o); return *this; }

    /**
     * Set the object if this Div is of type Object.
     * @param o object
     */
    Div& operator=(Component* o) { Object(o); return *this; }

    /**
     * Set the object if this Div is of type Object.
     * @param o object
     */
    Div& operator=(const Settings& o) { Padding(o.padding); Divs(o.divs); Align(o.align); DivSize(o.size); Dividers(o.dividers); return *this; }
    
    /**
     * Set the object if this Div is of type Object.
     * @param o object
     */
    Div& operator=(const ObjectSettings& o) { Object(o.component); DivSize(o.size); Align(o.align); ResizeComponent(o.resize); return *this; }

    /**
     * Clear this div and all sub divs.
     */
    void Clear()
    {
        m_Divs.clear();
        m_Object = nullptr;
        m_Cells = 0;
    }

private:
    std::vector<std::unique_ptr<Div>> m_Divs;
    Component* m_Object;
    Type m_Type = Type::Divs;
    Alignment m_Align = Alignment::Center;
    int m_CellSize = AUTO;
    int m_Cells = 0;
    int m_Padding = 0;
    bool m_Dividers = false;
    bool m_ResizeComponent = false;
};

// --------------------------------------------------------------------------
// --------------------------- LayoutManager --------------------------------
// --------------------------------------------------------------------------

/**
 * Each <code>Panel</code> contains an instance of <code>LayoutManager</code> and it 
 * helps with arranging the components inside the <code>Panel</code>.
 */
struct LayoutManager
{
    /**
     * Constructor
     */
    LayoutManager();

    /**
     * The <code>LayoutManager</code> also receives events because it also manages the
     * resizability of some layouts, and thus needs to receive mouse events.
     * @param e event
     */
    void AddEvent(Event& e) { m_Listener.AddEvent(e); }

    /**
     * Requested cursor to be displayed, used when resizing.
     * @return cursor id
     */
    int Cursor() const { return m_Cursor; }
    bool WantsCursor() const { return m_Cursor != -1; }
    
    /**
     * Update the positions and sizes of the components using the given dimensions.
     * @param dim rectangle of the available space
     * @param components components
     */
    template<typename TypeCollection>
    void Update(const Vec4<int>& dim, TypeCollection& components) 
    {
        m_Cursor = -1;
        if (m_Layout == Layout::Grid) UpdateLayoutGrid(dim, components);
        else if (m_Layout == Layout::Stack) UpdateLayoutStack(dim, components);
        else if (m_Layout == Layout::SidewaysStack) UpdateLayoutSidewaysStack(dim, components);
        else if (m_Layout == Layout::Border) UpdateLayoutBorder(dim, components);
        else if (m_Layout == Layout::Divs) UpdateLayoutDivs(dim, components);
        m_MousePress = Event::MouseButton::NONE;
    }

    /**
     * Returns the <code>Layout</code> that is in use.
     * @return layout
     */
    ::Layout Layout() const{ return m_Layout; }

    /**
     * Set the <code>Layout</code>
     * @tparam T <code>Layout::Type</code>
     * @param ...args the arguments for the constructor of <code>Layout</code> with the given type <code>T</code>
     */
    template<Layout::Type T, typename ...Args>
    void Layout(Args&& ...args) { m_Layout = ::Layout{ T, args... }; }

    /**
     * The biggest coords that were seen by this <code>LayoutManger</code>, can be used by the
     * <code>Panel</code> to automatically resize.
     * @return biggest coords
     */
    Vec2<int> BiggestCoords() { return { m_BiggestX, m_BiggestY }; }

    ::Div& Div() { return m_Div; }

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
            int _x = dim.width - _eastWidth + _p2 + _p,
                _y = _southHeight,
                _w = _eastWidth - _p - _p2,
                _h = dim.height - _southHeight - _northHeight;

            // Detect mousepress on resize area
            if (m_MouseX > _x - _p && m_MouseX < _x &&
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
            if (m_MouseX > _x + _w && m_MouseX < _x + _w + _p &&
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
                    _w = std::min(_w + _diffo, _w + _diff);
                    _west->Width(_west->Width() - (_w - _pw));
                    _westWidth -= (_w - _pw);
                }

                // If east is resizable and resizing is not done yet
                if (_rE && !_done && _east && (_diffo = _east->Width() - _east->MinWidth()) > 0)
                {
                    // Adjust the widths
                    int _pw = _w;
                    _w = std::min(_w + _diffo, _w + _diff);
                    _east->Width(_east->Width() - (_w - _pw));
                    _east->X(_east->X() + (_w - _pw));
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
                    _h = std::min(_h + _diffo, _h + _diff);
                    _north->Height(_north->Height() - (_h - _ph));
                }

                // If it is not done yet and south is resizable
                if (_rS && !_done && _south && (_diffo = _south->Height() - _south->MinHeight()) > 0)
                {
                    // Adjust the heights
                    int _ph = _h;
                    _h = std::min(_h + _diffo, _h + _diff);
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

        int _paddingx = _stack.paddingx;
        int _paddingy = _stack.paddingy;
        int _y = dim.y;
        size_t _size = components.size();
        for (int _i = _size - 1; _i >= 0; _i--)
        {
            auto& _c = components[_i];
            if (!_c->Visible())
                continue;

            int _x = dim.x + _paddingx;
            _c->Position({ _x, _y });
            _c->Width(dim.width - 2 * _paddingx);
            _y += _c->Height() + _paddingy;
        }
        m_BiggestX = dim.x + dim.width;
        m_BiggestY = _y;
    }

    template<typename TypeCollection>
    void UpdateLayoutSidewaysStack(const Vec4<int>& dim, TypeCollection& components)
    {
        Layout::StackLayout& _stack = m_Layout.stack;

        int _paddingx = _stack.paddingx;
        int _paddingy = _stack.paddingy;
        int _x = dim.x + _paddingx;
        size_t _size = components.size();
        for (int _i = _size - 1; _i >= 0; _i--)
        {
            auto& _c = components[_i];
            if (!_c->Visible())
                continue;

            int _y = dim.y + _paddingy;
            _c->Position({ _x, _y });
            _c->Height(dim.height - 2 * _paddingy);
            _x += _c->Width() + _paddingx;
        }
        m_BiggestX = _x;
        m_BiggestY = dim.y + dim.height;
    }

    template<typename TypeCollection>
    void UpdateLayoutDivs(const Vec4<int>& dim, TypeCollection& components)
    {
        if (m_PrevDim == dim)
            return;

        m_PrevDim = dim;
        m_Dividers.clear();
        UpdateDiv(Div(), dim);
    }

    void UpdateDiv(::Div& div, const Vec4<int>& dim)
    {
        if (div.DivType() == Div::Type::Object)
            SetObject(div, dim);

        else
            if (div.Align() == Div::Alignment::Horizontal)
            {
                // Get all the sizes of the sub-divs
                std::vector<int> sizes;
                int width = dim.width, amt = 0, obamt = 0;
                for (auto& i : div.Divs())
                    if (i->DivSize() == Div::AUTO)
                    {
                        if (!i->ResizeComponent() && i->DivType() == Div::Type::Object)
                            sizes.push_back(i->Object().Size().width + i->Padding()), width -= i->Object().Size().width + i->Padding(), obamt++;
                        else
                            sizes.push_back(0), amt++;
                    }
                    else
                        width -= i->DivSize(), sizes.push_back(-i->DivSize());

                // See what's left to divide
                int x = dim.x;
                int w = width;
                if (amt)
                    w /= amt;
                else if (obamt)
                    w /= obamt;

                // If there's some space left, either give it to divs or objects
                if (w > 0)
                    for (auto& i : sizes)
                        if (amt && i == 0) // Give space to div with no given size
                            i = w;
                        else if (obamt && i > 0) // Give space to object
                            i += w;

                // if we're short on space, take some away from objects or divs.
                if (width < 0)
                    for (auto& i : sizes)
                        if (i > 0 && obamt) // Take away from object
                            i -= w;
                        else if (i < 0 && !obamt) // Take away from div with a given size
                            i -= w;

                // Set sizes and positions
                for (int i = 0; i < div.Divs().size(); i++)
                {
                    if (div.Dividers() && i != 0)
                        m_Dividers.emplace_back(Vec4<int>{ x, dim.y + div.Padding(), 1, dim.height - 2 * div.Padding() });

                    if (sizes[i] < 0) // If div with given space, size is negative so negate
                        UpdateDiv(*div.Divs()[i], { x + div.Padding(), dim.y + div.Padding(), -sizes[i] - 2 * div.Padding(), dim.height - 2 * div.Padding() }), x += -sizes[i];
                    else // Otherwise just recurse
                        UpdateDiv(*div.Divs()[i], { x + div.Padding(), dim.y + div.Padding(), sizes[i] - 2 * div.Padding(), dim.height - 2 * div.Padding() }), x += sizes[i];
                }
            }
            else
            {
                // Get all the sizes of the sub-divs
                std::vector<int> sizes;
                int height = dim.height, amt = 0, obamt = 0;
                for (auto& i : div.Divs())
                    if (i->DivSize() == Div::AUTO)
                    {
                        if (!i->ResizeComponent() && i->DivType() == Div::Type::Object)
                            sizes.push_back(i->Object().Size().height + i->Padding()), height -= i->Object().Size().height + i->Padding(), obamt++;
                        else
                            sizes.push_back(0), amt++;
                    }
                    else
                        height -= i->DivSize(), sizes.push_back(-i->DivSize());

                // See what's left to divide
                int y = dim.y;
                int h = height;
                if (amt)
                    h /= amt;
                else if (obamt)
                    h /= obamt;

                // If there's some space left, either give it to divs or objects
                if (h > 0)
                    for (auto& i : sizes)
                        if (amt && i == 0) // Give space to div with no given size
                            i = h;
                        else if (obamt && i > 0) // Give space to object
                            i += h;

                // if we're short on space, take some away from objects or divs.
                if (height < 0)
                    for (auto& i : sizes)
                        if (i > 0 && obamt) // Take away from object
                            i += h;
                        else if (i < 0 && !obamt) // Take away from div with a given size
                            i += h;

                // Set sizes and positions
                for (int i = 0; i < div.Divs().size(); i++)
                {
                    if (div.Dividers() && i != 0)
                        m_Dividers.emplace_back(Vec4<int>{ dim.x + div.Padding(), y, dim.width - 2 * div.Padding(), 1 });

                    if (sizes[i] < 0) // If div with given space, size is negative so negate
                        UpdateDiv(*div.Divs()[i], { dim.x + div.Padding(), y + div.Padding(), dim.width - 2 * div.Padding(), -sizes[i] - 2 * div.Padding() }), y += -sizes[i];
                    else // Otherwise just recurse
                        UpdateDiv(*div.Divs()[i], { dim.x + div.Padding(), y + div.Padding(), dim.width - 2 * div.Padding(), sizes[i] - 2 * div.Padding() }), y += sizes[i];
                }
            }
    }

    void SetObject(::Div& div, const Vec4<int>& dim)
    {
        // Get the object from the div
        Component* object = &div.Object();

        // Calculate the position using the alignment
        Vec2<int> position = { dim.x, dim.y };
        
        // If resize component, set size
        if (div.ResizeComponent())
        {
            if (object->Size() != dim.size)
                m_PrevDim = { 0, 0, 0, 0 },
                object->Size(dim.size),
                object->ConstrainSize();
        }

        // Otherwise adjust positioning
        else
        {
            if (div.Align() == Div::Alignment::Center)
                position += { dim.width / 2 - object->Size().width / 2, dim.height / 2 - object->Size().height / 2 };
            else if (div.Align() == Div::Alignment::Right)
                position += { dim.width - object->Size().width, dim.height / 2 - object->Size().height / 2 };
            else if (div.Align() == Div::Alignment::Left)
                position += { 0, dim.height / 2 - object->Size().height / 2 };
            else if (div.Align() == Div::Alignment::Bottom)
                position += { dim.width / 2 - object->Size().width / 2, 0 };
            else if (div.Align() == Div::Alignment::Top)
                position += { dim.width / 2 - object->Size().width / 2, dim.height - object->Size().height };
        }
        object->Position(position);

        

        if (object->Width() + position.x > m_BiggestX)
            m_BiggestX = object->Width() + position.x;

        if (object->Height() + position.y > m_BiggestY)
            m_BiggestY = object->Height() + position.y;
    }

    void DrawDividers(CommandCollection& d)
    {
        d.Command<Graphics::Fill>(m_DivColor);
        for (auto& i : m_Dividers)
            d.Command<Graphics::Quad>(i);
    }

    void DividerColor(const Color& c) { m_DivColor = c; }

    void Refresh() { m_PrevDim = { 0, 0, 0, 0 }; }

    int m_Constrain(int a, int b, int c)
    {
        if (c == -1)
            return a < b ? b : a;
        else
            return a < b ? b : a > c ? c : a;
    }


    ::Layout m_Layout = { ::Layout::Free };
    EventListener m_Listener;
    ::Div m_Div;

    Color m_DivColor;
    std::vector<Vec4<int>> m_Dividers;
    Vec4<int> m_PrevDim;

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