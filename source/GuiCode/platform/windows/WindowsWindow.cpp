#include "GuiCode/platform/windows/WindowsWindow.hpp"
#include "GuiCode/ui/events/Event.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"
#include "GuiCode/ui/RightClickMenu.hpp"
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

namespace PlatformFunctions
{
    Vec2<int> GlobalCursorPos()
    {
        POINT curPos; GetCursorPos(&curPos);
        return { curPos.x, curPos.y };
    }

    Vec4<int> MonitorRecFromPoint(const Vec2<int>& point)
    {
        auto monitor = MonitorFromPoint(POINT{ point.x, point.y }, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info;
        info.cbSize = sizeof(MONITORINFO);

        GetMonitorInfoA(monitor, &info);

        return { info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom };
    }
}


static int mini(int x, int y)
{
    return x < y ? x : y;
}

static int maxi(int x, int y)
{
    return x > y ? x : y;
}

GLFWmonitor* get_current_monitor(GLFWwindow* window)
{
    int nmonitors, i;
    int wx, wy, ww, wh;
    int mx, my, mw, mh;
    int overlap, bestoverlap;
    GLFWmonitor* bestmonitor;
    GLFWmonitor** monitors;
    const GLFWvidmode* mode;

    bestoverlap = 0;
    bestmonitor = NULL;

    glfwGetWindowPos(window, &wx, &wy);
    glfwGetWindowSize(window, &ww, &wh);
    monitors = glfwGetMonitors(&nmonitors);

    for (i = 0; i < nmonitors; i++) {
        mode = glfwGetVideoMode(monitors[i]);
        glfwGetMonitorPos(monitors[i], &mx, &my);
        mw = mode->width;
        mh = mode->height;

        overlap =
            maxi(0, mini(wx + ww, mx + mw) - maxi(wx, mx)) *
            maxi(0, mini(wy + wh, my + mh) - maxi(wy, my));

        if (bestoverlap < overlap) {
            bestoverlap = overlap;
            bestmonitor = monitors[i];
        }
    }

    return bestmonitor;
}

int WindowsWindow::m_WindowCount = 0;
int WindowsWindow::m_WindowIdCounter = 0;
WindowsWindow* WindowsWindow::m_MainWindow = nullptr;
std::vector<NOTIFYICONDATA> WindowsWindow::m_ShellIcons;
int WindowsWindow::m_ShellIconCount = 0;
std::unordered_map<int, std::function<void(Event&)>> WindowsWindow::m_ShellIconCallbacks;
WindowsWindow* WindowsWindow::CurrentWindow = nullptr;

WindowsWindow::WindowsWindow(const WindowData& d)
    : WindowBase(d)
{
    m_InitialSize = d.size;
    m_Projection = glm::ortho(0.0f, (float)std::max(d.size.width / m_Scale, 5.0f), 0.0f, (float)std::max(d.size.height / m_Scale, 5.0f));
    if (m_WindowCount == 0 && !glfwInit())
    {
        LOG("Failed to initialize GLFW");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FLOATING, d.alwaysOnTop);
    glfwWindowHint(GLFW_RESIZABLE, d.resizeable);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, d.transparentBuffer);
    glfwWindowHint(GLFW_DECORATED, d.decorated);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_VISIBLE, d.showOnCreate);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

    auto window = dynamic_cast<Window*>(d.parent);
    m_Window = glfwCreateWindow(55, 55, d.name.c_str(), NULL, m_MainWindow ? m_MainWindow->operator GLFWwindow *() : NULL);
    if (window)
        SetWindowLongPtr(GetWin32Handle(), GWLP_HWNDPARENT, (LONG)window->GetWin32Handle());

    m_WindowId = m_WindowIdCounter++;
    if (&m_Window == NULL)
    {
        LOG("Failed to create GLFW window");
        if (--m_WindowCount == 0)
            glfwTerminate();
    }

    if (!d.decorated)
    {
        long style = GetWindowLong(GetWin32Handle(), GWL_EXSTYLE);
        style = style & ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW; 
        SetWindowLong(GetWin32Handle(), GWL_EXSTYLE, style);
    }

    glfwMakeContextCurrent(*this);

    if (m_WindowCount == 0)
        WindowsWindow::m_MainWindow = this;

    if (m_WindowCount == 0 && !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG("Failed to initialize GLAD");
        exit(-1);
    }

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_SCISSOR_TEST);
    glfwSwapInterval(0);

    m_WindowCount++;

    // Add a subproc to handle some msgs ourselves.
    SetWindowSubclass(GetWin32Handle(), &SubClassProc, m_WindowId, (DWORD_PTR) static_cast<void*>(this));

    // Set the margins of the Win32 window.
    MARGINS _margins = { 1, 1, 1, 1 };
    //if (!shadow)
    //    _margins = { 0, 0, 0, 0 };
    DwmExtendFrameIntoClientArea(GetWin32Handle(), &_margins);

    //Size(Width(), Height());    
    d.showOnCreate ? Show() : Hide();

}

void WindowsWindow::FullScreen(bool f)
{
    if (FullScreen() == f)
        return;

    if (f)
    {
        m_BackupDims = { Location(), RealSize() };
        m_BackupDims.x += 8;
        m_BackupDims.y += 32;
        m_BackupDims.width -= 16;
        m_BackupDims.height -= 40;

        // get resolution of monitor
        auto _monitor = get_current_monitor(m_Window);
        const GLFWvidmode* mode = glfwGetVideoMode(_monitor);

        // switch to full screen
        glfwSetWindowMonitor(m_Window, _monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        // restore last window size and position
        glfwSetWindowMonitor(m_Window, nullptr, m_BackupDims.x, m_BackupDims.y, m_BackupDims.width, m_BackupDims.height, 0);
    }

}

void WindowsWindow::Update(const Vec4<int>& viewport) 
{
    // Update all the components
    Container::Update(viewport);

    // Update the cursor after updating the components
    UpdateCursor(Cursor());
}

void WindowsWindow::Render(CommandCollection& d)
{
    using namespace Graphics;
    d.Command<Clip>(0, 0, Width(), Height());
    d.Command<Type::Viewport>(0, 0, Width(), Height());
    d.Command<Fill>(m_Color);
    d.Command<Quad>(0, 0, Width(), Height());
    WindowBase::Render(d);
    d.Command<PopClip>();
}

void WindowsWindow::Loop()
{
    CurrentWindow = this;
    WindowsLoop();
    glfwPollEvents();
}

void WindowsWindow::WindowsLoop()
{
    static int _currentId = -1;

    // Only change context if it's necessary, because this seems to
    // have a relatively high impact on the CPU usage.
    if (_currentId != m_WindowId && Visible())
    {
        _currentId = m_WindowId;
        glfwMakeContextCurrent(*this);
    }

    if (Visible())
    {
        if (RightClickMenu::Get().GetWin32Handle() != GetWin32Handle() && RightClickMenu::Get().Opened())
            m_MousePressed = Event::MouseButton::NONE;

        Graphics::CurrentWindow(m_WindowId);
        Graphics::Scaling(m_Scale);
        Graphics::WindowFocused(GetForegroundWindow() == GetWin32Handle());
        Graphics::SetProjection(m_Projection);

        Update({ 0, 0, Width(), Height() });
    }
    
    if (Visible())
    {

        //d.Command<Graphics::FrameBuffer>(9999, true, Vec4<int>{0, 0, Width(), Height()});
        CommandCollection d;
        d.Get().reserve(m_EstimateCommandSize);
        Render(d);
        m_EstimateCommandSize = d.Get().size() + 100;
        //d.Command<Graphics::FrameBufferEnd>();
        Graphics::RunCommands(d);
        glfwSwapBuffers(*this);


        // Need to trigger a resize after setup because otherwise
        // stuff fails for some reason...
        if (Visible() && m_InitialResize)
        {
            m_InitialResize = false;
            Size(m_InitialSize);
        }

        // Go through the even queue
        while (!m_EventQueue.empty())
        {
            AddEvent(m_EventQueue.front());

            // Check again if the eventqueue is empty, because some button somewhere
            // might trigger a resize of the window, which will trigger this WindowsWindow::Update
            // which will finish this while loop, and then exit out of AddEvent about here ^
            // and then m_EventQueue will be empty, causing an exception when calling pop().
            if (!m_EventQueue.empty())
                m_EventQueue.pop();
        }
    }

}

int WindowsWindow::AddHotKey(Key k, const WindowsWindow::Callback& c)
{
    int value = k;
    int key = value >> 8;
    int mod = value & 0xff;
    int winMod = 0;
    int id = m_HotkeyCounter++;

    if (mod & Event::Mod::CONTROL) { winMod |= MOD_CONTROL; }
    if (mod & Event::Mod::ALT) { winMod |= MOD_ALT; }
    if (mod & Event::Mod::SHIFT) { winMod |= MOD_SHIFT; }

    if (RegisterHotKey(GetWin32Handle(), id, winMod, key))
        m_Hotkeys.emplace(id, c);

    return id;
}

void WindowsWindow::RemoveHotKey(int id)
{
    m_Hotkeys.erase(id);
    UnregisterHotKey(GetWin32Handle(), id);
}

// Hit test the frame for resizing and moving.
LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam, float scale)
{
    // Get the point coordinates for the hit test.
    POINT _ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    // Get the window rectangle.
    RECT _rcWindow;
    GetWindowRect(hWnd, &_rcWindow);

    // Get the frame rectangle, adjusted for the style without a caption.
    RECT _rcFrame = { 0 };
    AdjustWindowRectEx(&_rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

    // Determine if the hit test is for resizing. Default middle (1,1).
    USHORT _uRow = 1;
    USHORT _uCol = 1;
    bool _fOnResizeBorder = false;

    // Determine if the point is at the top or bottom of the window.
    int _padding = 8 / scale;
    if (_ptMouse.y >= _rcWindow.top && _ptMouse.y < _rcWindow.top + 32 / scale)
    {
        _fOnResizeBorder = (_ptMouse.y < (_rcWindow.top - _rcFrame.top));
        _uRow = 0;
    }
    else if (_ptMouse.y < _rcWindow.bottom && _ptMouse.y >= _rcWindow.bottom - _padding)
    {
        _uRow = 2;
    }

    // Determine if the point is at the left or right of the window.
    if (_ptMouse.x >= _rcWindow.left && _ptMouse.x < _rcWindow.left + _padding)
        _uCol = 0; // left side

    else if (_ptMouse.x < _rcWindow.right && _ptMouse.x >= _rcWindow.right - _padding)
        _uCol = 2; // right side

    // Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
    LRESULT _hitTests[3][3] =
    {
        { HTTOPLEFT,    _fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
        { HTLEFT,       HTNOWHERE,     HTRIGHT },
        { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
    };

    return _hitTests[_uRow][_uCol];
}

LRESULT CALLBACK WindowsWindow::SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    static bool _resizing = true;
    WindowsWindow* _self = (WindowsWindow*)dwRefData;
    if (_self == nullptr)
        return 0;
    bool _fCallDWP = true;
    LRESULT _lRet = 0;

    // ShellIconCallback
    if (uMsg == 10000 && m_ShellIconCallbacks.find(wParam) != m_ShellIconCallbacks.end())
    {
        if (lParam == WM_LBUTTONDOWN)
        {
            Event _e{ Event::Type::MousePressed, 0, 0, Event::MouseButton::LEFT };
            m_ShellIconCallbacks[wParam](_e);
        }
        if (lParam == WM_LBUTTONDBLCLK)
        {
            Event _e{ Event::Type::MousePressed, 0, 0, Event::MouseButton::LEFT, true };
            m_ShellIconCallbacks[wParam](_e);
        }
        if (lParam == WM_RBUTTONDOWN)
        {
            Event _e{ Event::Type::MousePressed, 0, 0, Event::MouseButton::RIGHT };
            m_ShellIconCallbacks[wParam](_e);
        }
        if (lParam == WM_RBUTTONDBLCLK)
        {
            Event _e{ Event::Type::MousePressed, 0, 0, Event::MouseButton::RIGHT, true };
            m_ShellIconCallbacks[wParam](_e);
        }
    }

    // Winproc worker for custom frame issues.
    _fCallDWP = !DwmDefWindowProc(hWnd, uMsg, wParam, lParam, &_lRet);

    // Buttons if not on the edge or top bar
    int _offset = IsMaximized(hWnd) ? -8 : 0;
    double _padding = 8.0 / _self->m_Scale;
    if (_self->m_HoveringComponent != nullptr && _self->CursorPos().y > _self->Height() - (32 + _offset) / _self->m_Scale || _self->m_MousePressed != Event::MouseButton::NONE ||
        _self->CursorPos().y < _self->Height() - (32 + _offset) / _self->m_Scale && _self->CursorPos().x > _padding && _self->CursorPos().x < _self->Width() - _padding && _self->CursorPos().y > _padding)
    {
        bool _dblclk = false;
        bool _rClick = false;
        bool _click = false;
        switch (uMsg)
        {
        case WM_NCLBUTTONDBLCLK:
        case WM_LBUTTONDBLCLK:
            _dblclk = true;
            [[fallthrough]];
        case WM_NCLBUTTONDOWN:
        case WM_LBUTTONDOWN:
            SetCapture(hWnd);
            _self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MousePressed, _dblclk);
            _fCallDWP = false;
            _click = true;
            break;

        case WM_NCLBUTTONUP:
        case WM_LBUTTONUP:
            _self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MouseReleased, 0);
            ReleaseCapture();
            _fCallDWP = false;
            _click = true;
            break;

        case WM_NCRBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
            _dblclk = true;
            [[fallthrough]];
        case WM_NCRBUTTONDOWN:
        case WM_RBUTTONDOWN:
            SetCapture(hWnd);
            _self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MousePressed, _dblclk);
            _fCallDWP = false;
            _click = true;
            break;

        case WM_NCRBUTTONUP:
        case WM_RBUTTONUP:
            _rClick = true;
            _self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MouseReleased, 0);
            ReleaseCapture();
            _fCallDWP = false;
            _click = true;
            break;

        case WM_NCMBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
            _dblclk = true;
            [[fallthrough]];
        case WM_NCMBUTTONDOWN:
        case WM_MBUTTONDOWN:
            SetCapture(hWnd);
            _self->MouseButtonCallback(_self, Event::MouseButton::MIDDLE, Event::Type::MousePressed, _dblclk);
            _fCallDWP = false;
            _click = true;
            break;

        case WM_NCMBUTTONUP:
        case WM_MBUTTONUP:
            _self->MouseButtonCallback(_self, Event::MouseButton::MIDDLE, Event::Type::MouseReleased, 0);
            ReleaseCapture();
            _fCallDWP = false;
            _click = true;
            break;
        }
        if (_rClick && RightClickMenu::Get().Opened() && _self != &RightClickMenu::Get());
        else
        if (_click && _fCallDWP == false && GetFocus() != _self->GetWin32Handle())
            ::SetFocus(_self->GetWin32Handle());
    }
    else
    {
        
        // Cheaty instant click when pressing on the window dragging part of the title bar
        switch (uMsg)
        {
        case WM_NCLBUTTONDOWN:
            RightClickMenu::Get().Close();
        case WM_LBUTTONDOWN:
            _self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MousePressed, 0);
            _self->MouseButtonCallback(_self, Event::MouseButton::LEFT, Event::Type::MouseReleased, 0);
            break;

        case WM_NCRBUTTONDOWN:
            RightClickMenu::Get().Close();
        case WM_RBUTTONDOWN:
            _self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MousePressed, 0);
            _self->MouseButtonCallback(_self, Event::MouseButton::RIGHT, Event::Type::MouseReleased, 0);
            break;

        case WM_NCMBUTTONDOWN:
            RightClickMenu::Get().Close();
        case WM_MBUTTONDOWN:
            _self->MouseButtonCallback(_self, Event::MouseButton::MIDDLE, Event::Type::MousePressed, 0);
            _self->MouseButtonCallback(_self, Event::MouseButton::MIDDLE, Event::Type::MouseReleased, 0);
            break;
        }
    }

    switch (uMsg)
    {
    case WM_HOTKEY:
    {
        _self->m_Hotkeys[wParam]();
    }
    break;
    case WM_WINDOWPOSCHANGED:
    {
        //if (IsWindowVisible(hWnd) == true)
        //    SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
    }
    break;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSCHAR:
    case WM_CHAR:
    {
        int _mod = ((GetKeyState(VK_CONTROL) & 0x8000) ? Event::Mod::CONTROL : 0)
            | (((GetKeyState(VK_MENU) & 0x8000) || GetKeyState(VK_RMENU) & 0x8000) ? Event::Mod::ALT : 0)
            | ((GetKeyState(VK_SHIFT) & 0x8000) ? Event::Mod::SHIFT : 0)
            | ((GetKeyState(VK_NUMLOCK) & 0x8000) ? Event::Mod::NUM_LOCK : 0)
            | ((GetKeyState(VK_CAPITAL) & 0x8000) ? Event::Mod::CAPS_LOCK : 0);

        int _repeat = (lParam & 0x40000000);
        _self->KeyCallback(_self, wParam, _repeat, 
            uMsg == WM_SYSKEYDOWN || uMsg == WM_KEYDOWN ? Event::Type::KeyPressed : 
            uMsg == WM_CHAR || uMsg == WM_SYSCHAR ? Event::Type::KeyTyped : Event::Type::KeyReleased, _mod);
        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        RECT _rect;
        long _x = 0, _y = 0;
        if (::GetWindowRect(hWnd, &_rect))
            _x = _rect.left, _y = _rect.top;

        int _win32mod = GET_KEYSTATE_WPARAM(wParam);
        int _mod = ((_win32mod & 0x0008) ? Event::Mod::CONTROL : 0)
            | ((_win32mod & 0x0001) ? Event::Mod::LEFT : 0)
            | ((_win32mod & 0x0010) ? Event::Mod::MIDDLE: 0)
            | ((_win32mod & 0x0002) ? Event::Mod::RIGHT : 0)
            | ((_win32mod & 0x0004) ? Event::Mod::SHIFT: 0);
        int _amount = GET_WHEEL_DELTA_WPARAM(wParam);
        int _xPos = GET_X_LPARAM(lParam);
        int _yPos = GET_Y_LPARAM(lParam);
        _self->MouseWheelCallback(_self, _amount, _mod, _xPos - _x, _yPos - _y);
        break;
    }
    case WM_EXITSIZEMOVE:
    case WM_SIZE:
    case WM_SIZING:
    {           
        RECT _rect;
        int _width = 0, _height = 0;

        if (::GetWindowRect(hWnd, &_rect))
        {
            _width = _rect.right - _rect.left;
            _height = _rect.bottom - _rect.top;
        }
        _self->WindowSizeCallback(_self, _width, _height);
        break;
    }
    case WM_MOUSEMOVE:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        _self->CursorPosCallback(_self, x, y);
        break;
    }
    case WM_MOUSELEAVE:
        [[fallthrough]];
    case WM_NCMOUSELEAVE:
    {
        POINT pos;
        RECT rect;
        GetCursorPos(&pos);
        ::GetWindowRect(hWnd, &rect);
        int x = pos.x - rect.left;
        int y = pos.y - rect.top;
        _self->CursorPosCallback(_self, x, y);
        break;
    }
    case WM_NCMOUSEMOVE:
    {
        RECT _rect;
        long _x = 0, _y = 0;
        if (::GetWindowRect(hWnd, &_rect))
            _x = _rect.left, _y = _rect.top;

        _self->CursorPosCallback(_self, GET_X_LPARAM(lParam) - _x, GET_Y_LPARAM(lParam) - _y);
        break;
    }
    // Handle the non-client size message.
    case WM_NCCALCSIZE:
    {
        if (wParam == TRUE)
            _fCallDWP = false;
        break;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = _self->m_MinSize.width == -1 ? 0 : _self->m_MinSize.width;
        lpMMI->ptMinTrackSize.y = _self->m_MinSize.height == -1 ? 0 : _self->m_MinSize.height;
        lpMMI->ptMaxTrackSize.x = _self->m_MaxSize.width == -1 ? 100000 : _self->m_MaxSize.width;
        lpMMI->ptMaxTrackSize.y = _self->m_MaxSize.height == -1 ? 100000 : _self->m_MaxSize.height;
        _fCallDWP = false;
        _lRet = 0;
    }
    // Handle hit testing in the NCA if not handled by DwmDefWindowProc.
    case WM_NCHITTEST:
        if (!_self->FullScreen())
        if (_lRet == 0 && (_lRet = HitTestNCA(hWnd, wParam, lParam, _self->m_Scale)) != HTNOWHERE)
            _fCallDWP = false;
    }

    // Winproc worker for the rest of the application.
    if (_fCallDWP)
        _lRet = DefSubclassProc(hWnd, uMsg, wParam, lParam);

    return _lRet;
}

// --------------------------------------------------------------------------
// ---------------------------- Callbacks -----------------------------------
// --------------------------------------------------------------------------

void WindowsWindow::CursorPosCallback(WindowsWindow* window, int x, int y)
{
    auto _self = window;

    y = _self->m_Size.height - y;

    _self->m_MouseX = x * _self->m_Scale;
    _self->m_MouseY = y * _self->m_Scale;

    if (_self->m_MousePressed != Event::MouseButton::NONE)
        _self->m_EventQueue.emplace(Event::Type::MouseDragged, _self->m_MouseX, _self->m_MouseY, _self->m_MousePressed);
    else
        _self->m_EventQueue.emplace(Event::Type::MouseMoved, _self->m_MouseX, _self->m_MouseY);
}

void WindowsWindow::MouseButtonCallback(WindowsWindow* window, Event::MouseButton button, Event::Type action, int mod)
{
    auto _self = window;

    if (action == Event::Type::MouseReleased && _self->m_MousePressed != Event::MouseButton::NONE
        && _self->m_PressMouseX == _self->m_MouseX && _self->m_PressMouseY == _self->m_MouseY)
        _self->m_EventQueue.emplace(Event::Type::MouseClicked, _self->m_MouseX, _self->m_MouseY, button, mod);

    if (action == Event::Type::MousePressed)
    {
        _self->m_PressMouseX = _self->m_MouseX;
        _self->m_PressMouseY = _self->m_MouseY;
        _self->m_MousePressed = button;
    }
    else
        _self->m_MousePressed = Event::MouseButton::NONE;

    _self->m_EventQueue.emplace(action, _self->m_MouseX, _self->m_MouseY, button, mod);
}

void WindowsWindow::MouseWheelCallback(WindowsWindow* window, int amount, int mod, int x, int y)
{
    auto _self = window;

    _self->m_EventQueue.emplace(Event::Type::MouseWheel, x, y, amount, mod);
}

void WindowsWindow::KeyCallback(WindowsWindow* window, int key, int repeat, Event::Type action, int mod)
{
    auto _self = window;
    
    if (action == Event::Type::KeyTyped)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
        _self->m_EventQueue.emplace(action, conversion.to_bytes(key)[0], mod, ((char)key) == 0xffff);
    }

    else
        _self->m_EventQueue.emplace(action, key, mod, repeat);
}

void WindowsWindow::WindowSizeCallback(WindowsWindow* window, int width, int height)
{
    auto _self = window;

    _self->m_Size.width = width;
    _self->m_Size.height = height;

    _self->m_Projection = glm::ortho(0.0f, (float)std::max(width * _self->m_Scale, 5.0f), 0.0f, (float)std::max(height * _self->m_Scale, 5.0f));

    _self->WindowsLoop();
}

void WindowsWindow::UpdateCursor(int c)
{
    if (m_Cursorid == c)
        return;

    m_Cursorid = c;

    if (m_GLFWCursor)
        glfwDestroyCursor(m_GLFWCursor);
    if (m_Cursorid == -1)
        glfwSetCursor(*this, m_GLFWCursor = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL));
    else
        glfwSetCursor(*this,
            (m_GLFWCursor = glfwCreateStandardCursor(m_Cursorid)));
}


void WindowsWindow::Aero(bool b)
{
    if (m_Aero == b)
        return;

    m_Aero = b;
    const HINSTANCE _hModule = LoadLibrary(TEXT("user32.dll"));
    if (_hModule)
    {
        struct ACCENTPOLICY
        {
            int nAccentState; int nFlags; int nColor; int nAnimationId;
        };
        struct WINCOMPATTRDATA
        {
            int nAttribute; PVOID pData; ULONG ulDataSize;
        };
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(_hModule, "SetWindowCompositionAttribute");

        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { b ? 3 : 0, 0, 0, 0 }; // ACCENT_ENABLE_BLURBEHIND=3...
            WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) }; // WCA_ACCENT_POLICY=19
            SetWindowCompositionAttribute(GetWin32Handle(), &data);
        }

        FreeLibrary(_hModule);
    }
}

void WindowsWindow::Icon(int id)
{
    HICON hWindowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(id));
    SendMessage(GetWin32Handle(), WM_SETICON, ICON_BIG, (LPARAM)hWindowIcon);
    SendMessage(GetWin32Handle(), WM_SETICON, ICON_SMALL, (LPARAM)hWindowIcon);
}