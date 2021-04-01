#pragma once
#include "GuiCode/pch.hpp"

// --------------------------------------------------------------------------
// -------------------------------- Event -----------------------------------
// --------------------------------------------------------------------------

struct Event
{
    /**
     * Event type.
     */
    enum class Type
    {
        KeyTyped, KeyPressed, KeyReleased, MousePressed, MouseReleased,
        MouseClicked, MouseMoved, MouseDragged, MouseWheel, 
        MouseExited, MouseEntered, Focused, Unfocused
    };

    Type type;

    /**
     * Mouse button
     */
    enum class MouseButton { NONE = -1, LEFT, RIGHT, MIDDLE };

    /**
     * Event mods
     */
    enum Mod {
        SHIFT = 0x0001, CONTROL = 0x0002, ALT = 0x0004, SUPER = 0x0008,
        CAPS_LOCK = 0x0010, NUM_LOCK = 0x0020, LEFT = 0x0100, MIDDLE = 0x0200, RIGHT = 0x0400
    };

    /**
     * Constructor for <code>Focused</code> and <code>Unfocused</code> events.
     * @param t type
     */
    Event(Type t)
        : type(t)
    {}

    /**
     * Constructor for <code>MouseMoved</code>, <code>MouseEntered</code>, <code>MouseExited</code>, and <code>KeyReleased</code> events.
     * @param t type
     * @param x x or key
     * @param y y or keymod
     */
    Event(Type t, int x, int y)
        : type(t), x(x), y(y)
    {}

    /**
     * Constructor for <code>KeyPressed</code> events.
     * @param t type
     * @param key key 
     * @param keymod keymod
     * @param repeat repeat
     */
    Event(Type t, int key, int keymod, int repeat)
        : type(t), x(key), y(keymod), repeat(repeat)
    {}

    /**
     * Constructor for <code>KeyTyped</code> events.
     * @param t type
     * @param c char
     * @param keymod keymod
     */
    Event(Type t, char c, int keymod, int coded)
        : type(t), key(c), keymod(keymod), coded(coded)
    {}

    /**
     * Constructor for <code>MouseDragged</code> events.
     * @param t type
     * @param x x
     * @param y y
     * @param b mouse button
     */
    Event(Type t, int x, int y, MouseButton b)
        : type(t), x(x), y(y), button(b)
    {}

    /**
     * Constructor for <code>MousePressed</code>, <code>MouseReleased</code>, and <code>MouseClicked</code> events.
     * @param t type
     * @param x x
     * @param y y
     * @param b mouse button
     * @param m event mods
     */
    Event(Type t, int x, int y, MouseButton b, int m)
        : type(t), x(x), y(y), button(b), mod(m)
    {}

    /**
     * Constructor for <code>MouseWheel</code> events.
     * @param t type
     * @param x x
     * @param y y
     * @param a amount scrolled
     * @param m event mods
     */
    Event(Type t, int x, int y, int a, int m)
        : type(t), x(x), y(y), amount(a), wheelmod(m)
    {}

    // --------------------------------------------------------------------------
    // ---------------------------- Key Events ----------------------------------
    // --------------------------------------------------------------------------

    struct KeyPressed { int key; int keymod; int repeat, b; };
    struct KeyTyped { char key; int keymod, coded, c; };
    struct KeyReleased { int key; int keymod; int a, b; };

    // --------------------------------------------------------------------------
    // --------------------------- Mouse Events ---------------------------------
    // --------------------------------------------------------------------------

    struct MousePressed { int x, y; MouseButton button; int mod; };
    struct MouseReleased { int x, y; MouseButton button; int mod; };
    struct MouseClicked { int x, y; MouseButton button; int mod; };
    struct MouseMoved { int x, y, a, b; };
    struct MouseDragged { int x, y; MouseButton button; int a; };
    struct MouseWheel { int x, y, amount; int wheelmod; };
    struct MouseEntered { int x, y, a, b; };
    struct MouseExited { int x, y, a, b; };

    struct Focused { int a, b, c, d; };
    struct Unfocused { int a, b, c, d; };

    union
    {
        struct 
        {
            union { int key, x; };
            union { int keymod; int y; };
            union { MouseButton button; int amount, repeat, coded; };
            union { int mod; int wheelmod; };
        };

        KeyPressed keyPressed;
        KeyTyped keyTyped;
        KeyReleased keyReleased;

        MousePressed mousePressed;
        MouseReleased mouseReleased;
        MouseClicked mouseClicked;
        MouseMoved mouseMoved;
        MouseDragged mouseDragged;
        MouseWheel mouseWheel;
        MouseEntered mouseEntered;
        MouseExited mouseExited;

        Focused focused;
        Unfocused unfocused;
    };
};

// --------------------------------------------------------------------------
// --------------------------------- Key ------------------------------------
// --------------------------------------------------------------------------

class Key
{
public:
    /**
     * Type of key combo
     */
    enum Combo
    {
        NONE = 0,
        CTRL_A = ('A' << 8) | Event::Mod::CONTROL,
        CTRL_B = ('B' << 8) | Event::Mod::CONTROL,
        CTRL_C = ('C' << 8) | Event::Mod::CONTROL,
        CTRL_D = ('D' << 8) | Event::Mod::CONTROL,
        CTRL_E = ('E' << 8) | Event::Mod::CONTROL,
        CTRL_F = ('F' << 8) | Event::Mod::CONTROL,
        CTRL_G = ('G' << 8) | Event::Mod::CONTROL,
        CTRL_H = ('H' << 8) | Event::Mod::CONTROL,
        CTRL_I = ('I' << 8) | Event::Mod::CONTROL,
        CTRL_J = ('J' << 8) | Event::Mod::CONTROL,
        CTRL_K = ('K' << 8) | Event::Mod::CONTROL,
        CTRL_L = ('L' << 8) | Event::Mod::CONTROL,
        CTRL_M = ('M' << 8) | Event::Mod::CONTROL,
        CTRL_N = ('N' << 8) | Event::Mod::CONTROL,
        CTRL_O = ('O' << 8) | Event::Mod::CONTROL,
        CTRL_P = ('P' << 8) | Event::Mod::CONTROL,
        CTRL_Q = ('Q' << 8) | Event::Mod::CONTROL,
        CTRL_R = ('R' << 8) | Event::Mod::CONTROL,
        CTRL_S = ('S' << 8) | Event::Mod::CONTROL,
        CTRL_T = ('T' << 8) | Event::Mod::CONTROL,
        CTRL_U = ('U' << 8) | Event::Mod::CONTROL,
        CTRL_V = ('V' << 8) | Event::Mod::CONTROL,
        CTRL_W = ('W' << 8) | Event::Mod::CONTROL,
        CTRL_X = ('X' << 8) | Event::Mod::CONTROL,
        CTRL_Y = ('Y' << 8) | Event::Mod::CONTROL,
        CTRL_Z = ('Z' << 8) | Event::Mod::CONTROL,
        CTRL_COMMA = (188 << 8) | Event::Mod::CONTROL, // TODO: This one doesnt work
        CTRL_SHIFT_S = ('S' << 8) | Event::Mod::CONTROL | Event::Mod::SHIFT,
        CTRL_SHIFT_W = ('W' << 8) | Event::Mod::CONTROL | Event::Mod::SHIFT,
        ALT_F4 = (VK_F4 << 8) | Event::Mod::ALT,
        CTRL_F4 = (VK_F4 << 8) | Event::Mod::CONTROL,
        SHIFT_TAB = (0x9 << 8) | Event::Mod::SHIFT,
    };

    enum Value
    {
        BACKSPACE = 0x8,
        ESC = VK_ESCAPE,
        TAB = 0x9,
        DEL = 0x2e,
        ENTER = 0xd,
        LEFT = 0x25,
        RIGHT = 0x27,
        UP = 0x26,
        DOWN = 0x28,
        SHIFT = 0x10
    };
    
    /**
     * Constructor
     */
    Key() 
    {};

    /**
     * Constructor
     * @param v value
     */
    Key(Key::Combo v)
        : m_Value(v)
    {}

    /**
     * Constructor
     * v keytyped event, automatically converted to a key combo
     */
    Key(Event::KeyTyped v)
        : m_Value((Combo)((v.key << 8) | v.keymod))
    {}

    /**
     * Constructor
     * v keypressed event, automatically converted to a key combo
     */
    Key(Event::KeyPressed v)
        : m_Value((Combo)((v.key << 8) | v.keymod))
    {}

    /**
     * Constructor
     * v keyreleased event, automatically converted to a key combo
     */
    Key(Event::KeyReleased v)
        : m_Value((Combo)((v.key << 8) | v.keymod))
    {}

    bool operator==(Key::Combo a) const { return m_Value == a; }
    bool operator!=(Key::Combo a) const { return m_Value != a; }
    bool operator<(Key::Combo a)  const { return m_Value < a; }
    bool operator>(Key::Combo a)  const { return m_Value > a; }
    bool operator<=(Key::Combo a) const { return m_Value <= a; }
    bool operator>=(Key::Combo a) const { return m_Value >= a; }
    explicit operator int()       const { return static_cast<int>(m_Value); }
    operator Combo()              const { return m_Value; }

    /**
     * Convert the key combo to a string.
     */
    inline const std::string& ToString()
    {
        static std::string _ctrl = "Ctrl+";
        static std::string _shift = "Shift+";
        static std::string _alt = "Alt+";
        static std::unordered_map<Key::Combo, std::string> _keys;
        _keys.reserve(33);

        std::unordered_map<Key::Combo, std::string>::iterator _it;
        if ((_it = _keys.find(m_Value)) == _keys.end())
        {
            int _value = static_cast<int>(m_Value);
            bool _c = _value & Event::Mod::CONTROL;
            bool _s = _value & Event::Mod::SHIFT;
            bool _a = _value & Event::Mod::ALT;
            int _l = (_value & 0xffffff00) >> 8;
            bool _f = _l >= 0x70 && _l <= 0x7B;
            
            std::string _str;
            _str.reserve(_c ? _ctrl.size() : 0 + _s ? _shift.size() : 0 + _a ? _alt.size() : 0 + _f ? 2 : 1);
            
            if (_c) _str += _ctrl;
            if (_s) _str += _shift;
            if (_a) _str += _alt;
            if (_f)
                _str = _str + "F" + std::to_string(_l - 0x6F);
            if (_l == 188) _str += ',';
            else if (!_f)
                _str += static_cast<char>(_l);

            return _keys.emplace(m_Value, std::move(_str)).first->second;
        }
        else 
        {
            return _it->second;
        }
    }

private:        
    Combo m_Value;
};

inline bool operator ==(const Event::KeyPressed& a, const Key b) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Event::KeyPressed&& a, const Key b) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Event::KeyReleased& a, const Key b) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Event::KeyReleased&& a, const Key b) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Event::KeyTyped& a, const Key b) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Event::KeyTyped&& a, const Key b) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Key b, const Event::KeyPressed& a) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Key b, const Event::KeyPressed&& a) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Key b, const Event::KeyReleased& a) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Key b, const Event::KeyReleased&& a) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Key b, const Event::KeyTyped& a) { return ((a.key << 8) | a.keymod) == (int)b; }
inline bool operator ==(const Key b, const Event::KeyTyped&& a) { return ((a.key << 8) | a.keymod) == (int)b; }

