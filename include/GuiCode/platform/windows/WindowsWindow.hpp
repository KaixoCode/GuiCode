#pragma once
#include "GuiCode/ui/window/WindowBase.hpp"

// --------------------------------------------------------------------------
// -------------------------- Windows Window --------------------------------
// --------------------------------------------------------------------------

class WindowsWindow;
class WindowsWindow : public WindowBase
{
public:
    WindowsWindow(const std::string& name, int width, int height, bool hideonclose, bool show, bool resizable, bool decorated);
    ~WindowsWindow() 
    {
        for (auto& a : m_ShellIcons)
            Shell_NotifyIconA(NIM_DELETE, &a);

        SetWindowSubclass(GetWin32Handle(), &SubClassProc, m_WindowId, (DWORD_PTR) static_cast<void*>(nullptr));
        if (--m_WindowCount == 0)
            glfwTerminate();
    }
    
    auto Size(int w, int h)    -> void override { glfwSetWindowSize(*this, w, h); }
    auto Size(Vec2<int> s)     -> void override { glfwSetWindowSize(*this, s.width, s.height); }
    auto Location(Vec2<int> s) -> void override { glfwSetWindowPos(*this, s.x, s.y); }
    auto Maximize()            -> void override { glfwMaximizeWindow(*this); }
    auto Restore()             -> void override { glfwRestoreWindow(*this); }
    auto Iconify()             -> void override { glfwIconifyWindow(*this); }
    auto Close()               -> void override { glfwSetWindowShouldClose(*this, GL_TRUE); }
    auto Show()                -> void override { ReleaseCapture(); ShowWindow(GetWin32Handle(), SW_SHOW); }
    auto Visible(bool b)       -> void override { WindowBase::Visible(b); }
    auto Hide()                -> void override { ShowWindow(GetWin32Handle(), SW_HIDE); }
    auto ShouldClose(bool b)   -> void override { glfwSetWindowShouldClose(m_Window, b); };
    auto Color(Color c)-> void { m_Color = c; }
    void Aero(bool b);
    void Icon(int);

    operator    GLFWwindow* ()   const { return GetWindow(); }
    GLFWwindow* GetWindow()      const { return m_Window; }
    HWND        GetWin32Handle() const { return glfwGetWin32Window(*this); }
    Vec2<int>   Size()           const override { return { Width(), Height() }; }
    Vec2<int>   Location()       const override { int x, y; glfwGetWindowPos(m_Window, &x, &y); return { x, y }; }
    Vec2<int>   CursorPos()      const override { return { m_MouseX, m_MouseY }; }
    int         Width()          const override { return m_Size.width; }
    int         Height()         const override { return m_Size.height; }
    bool        Maximized()      const override { return IsMaximized(GetWin32Handle()); }
    bool        Visible()        const override { return IsWindowVisible(GetWin32Handle()); }
    bool        ShouldClose()    const override { return glfwWindowShouldClose(m_Window); };
    bool        Aero()           const { return m_Aero; }

    void Loop() override;
    void WindowsLoop();
    void Update(const Vec4<int>& viewport) override;
    void Render(CommandCollection&) override;

    template<typename T>
    static void AddShellIcon(int id, const std::string& title, T callback)
    {
        // ERROR cant add shell icon without window.
        if (m_MainWindow == nullptr)
            return;

        NOTIFYICONDATA& nidApp = m_ShellIcons.emplace_back();

        HICON hMainIcon = (HICON)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(id));
        nidApp.cbSize = sizeof(NOTIFYICONDATA); // sizeof the struct in bytes
        nidApp.hWnd = (HWND)m_MainWindow->GetWin32Handle(); //handle of the window which will process this app. messages
        nidApp.uID = m_ShellIconCount++;       //ID of the icon that willl appear in the system tray
        nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP; //ORing of all the flags
        nidApp.hIcon = hMainIcon; // handle of the Icon to be displayed, obtained from LoadIcon
        nidApp.uCallbackMessage = 10000;
        nidApp.uVersion = NOTIFYICON_VERSION_4;
        strcpy(nidApp.szTip, title.c_str());
        Shell_NotifyIconA(NIM_ADD, &nidApp);
        m_ShellIconCallbacks.emplace(nidApp.uID, callback);
    }

private:
    bool m_InitialResize = true,
        m_Aero = false;

    Vec2<int> m_InitialSize;

    GLFWwindow* m_Window;

    ::Color m_Color{ 23, 23, 23, 255 };

    Event::MouseButton m_MousePressed = Event::MouseButton::NONE;
    int m_PressMouseX = 0,
        m_PressMouseY = 0,
        m_MouseX = 0,
        m_MouseY = 0,
        m_WindowId = -1,
        m_Cursorid = 0;
    
    GLFWcursor* m_GLFWCursor = 0;
    glm::mat4 m_Projection;

    std::queue<Event> m_EventQueue;

    void UpdateCursor(int);

    static void CursorPosCallback(WindowsWindow* window, int x, int y);
    static void MouseButtonCallback(WindowsWindow* window, Event::MouseButton button, Event::Type action, int mod);
    static void MouseWheelCallback(WindowsWindow* window, int amount, int mod, int x, int y);
    static void KeyCallback(WindowsWindow* window, int key, int scancode, Event::Type action, int mod);
    static void WindowSizeCallback(WindowsWindow* window, int width, int height);

    static LRESULT SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
    static int m_WindowCount, m_WindowIdCounter;
    static WindowsWindow* m_MainWindow;
    static std::vector<NOTIFYICONDATA> m_ShellIcons;
    static std::unordered_map<int, std::function<void(Event&)>> m_ShellIconCallbacks;
    static int m_ShellIconCount;
};




