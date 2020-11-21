#pragma once

// --------------------------------------------------------------------------
// ------------------------------- Layout -----------------------------------
// --------------------------------------------------------------------------

struct Layout 
{
    enum Type 
    {
        Grid, Stack, Border, Free
    };

    enum class Hint
    {
        None, North, South, East, West, Center
    };

    Layout(Type type)
        : type(type)
    {}
    
    Layout(Type type, int p)
        : type(type), border({ p, p, true, true, true, true })
    {}

    Layout(Type type, int p, bool r)
        : type(type), border({ p, p, r, r, r, r })
    {}

    Layout(Type type, int p, bool a, bool b, bool c, bool d)
        : type(type), border({ p, p, a, b, c, d })
    {}

    Layout(Type type, int p, int p2)
        : type(type), border({ p, p2 })
    {}

    Layout(Type type, int p, int p2, bool r)
        : type(type), border({ p, p2, r, r, r, r })
    {}

    Layout(Type type, int p, int p2, bool a, bool b, bool c, bool d)
        : type(type), border({ p, p2, a, b, c, d })
    {}

    Layout(Type type, int c, int r, int h, int v)
        : type(type), grid({ c, r, h, v })
    {}

    ~Layout() {}

    Type type;
    
    operator Type() const { return type; }

    struct GridLayout // Grid Layout
    {
        int cols, rows, hgap, vgap;
    };

    struct BorderLayout // Grid Layout
    {
        int outerpadding, innerpadding;
        bool resizeN = true, resizeE = true, resizeS = true, resizeW = true;
    };

    struct StackLayout // Grid Layout
    {
        int padding;
    };

    union
    {
        GridLayout grid;
        BorderLayout border;
        StackLayout stack;
    };
};
