#pragma once

// --------------------------------------------------------------------------
// ------------------------------- Layout -----------------------------------
// --------------------------------------------------------------------------

struct Layout 
{
    /**
     * Layout type
     */
    enum Type 
    {
        /**
         * Simple grid layout with padding.
         */
        Grid, 
        
        /**
         * Vertical stack layout with padding.
         */
        Stack, 
        
        /**
         * Horizontal stack layout with padding.
         */
        SidewaysStack, 
        
        /**
         * Border layout, with a North, East, South, West, and Center component.
         */
        Border, 
        
        /**
         * Free roaming layout. Practically no layout.
         */
        Free
    };

    /**
     * A layout hint given to components that can be used by the <code>LayoutManager</code> 
     * to decide the location/size.
     */
    enum class Hint
    {
        None, North, South, East, West, Center
    };

    /**
     * Constructor
     * @param type
     */
    Layout(Type type)
        : type(type)
    {}
    
    /**
     * Constructor for BorderLayout
     * @param type
     * @param p padding
     */
    Layout(Type type, int p)
        : type(type), border({ p, p, true, true, true, true })
    {}

    /**
     * Constructor for BorderLayout
     * @param type
     * @param p padding
     * @param r resizable
     */
    Layout(Type type, int p, bool r)
        : type(type), border({ p, p, r, r, r, r })
    {}

    /**
     * Constructor for BorderLayout
     * @param type
     * @param p padding
     * @param a north resizable
     * @param b east resizable
     * @param c south resizable
     * @param d west resizable
     */
    Layout(Type type, int p, bool a, bool b, bool c, bool d)
        : type(type), border({ p, p, a, b, c, d })
    {}

    /**
     * Constructor for BorderLayout
     * @param type
     * @param p outer padding
     * @param p2 inner padding
     */
    Layout(Type type, int p, int p2)
        : type(type), border({ p, p2 })
    {}

    /**
     * Constructor for BorderLayout
     * @param type
     * @param p outer padding
     * @param p2 inner padding
     * @param r resizable
     */
    Layout(Type type, int p, int p2, bool r)
        : type(type), border({ p, p2, r, r, r, r })
    {}

    /**
     * Constructor for BorderLayout
     * @param type
     * @param p outer padding
     * @param p2 inner padding
     * @param a north resizable
     * @param b east resizable
     * @param c south resizable
     * @param d west resizable
     */
    Layout(Type type, int p, int p2, bool a, bool b, bool c, bool d)
        : type(type), border({ p, p2, a, b, c, d })
    {}

    /**
     * Constructor for GridLayout
     * @param type
     * @param c number of columns
     * @param r number of rows
     * @param h y padding
     * @param v x padding
     */
    Layout(Type type, int c, int r, int h, int v)
        : type(type), grid({ c, r, h, v })
    {}
    
    ~Layout() {}

    Type type;
    
    /**
     * Implicit operator to Layout::Type.
     */
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
