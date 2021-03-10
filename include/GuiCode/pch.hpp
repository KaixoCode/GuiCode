#pragma once
#include <glad/glad.hpp>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <ft2build.h>
#include FT_FREETYPE_H  

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <array>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

#define constrain(x, y, z) (x < y ? y : x > z ? z : x)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#include <unknwn.h>
//#include <gdiplus.h>
#include <windowsx.h>
#define MYWM_NOTIFYICON (WM_APP + 100 )

// Rarely updated local headers
#include "GuiCode/data/Vec.hpp"

//struct Color : public Vec4<float>
//{
//    constexpr Color(const Color& o)
//        : Vec4<float>{ o.r, o.g, o.b, o.a }
//    {}
//
//    constexpr Color(const Vec4<float>& o)
//        : Vec4<float>{ o.r, o.g, o.b, o.a }
//    {}
//
//    constexpr Color(Color&& o)
//        : Vec4<float>{ o.r, o.g, o.b, o.a }
//    {}
//
//    constexpr Color(float g)
//        : Vec4<float>{ g, g, g, 255 }
//    {}
//
//    constexpr Color(float g, float a)
//        : Vec4<float>{ g, g, g, a }
//    {}
//
//    constexpr Color(float r, float g, float b)
//        : Vec4<float>{ r, g, b, 255 }
//    {}
//
//    constexpr Color(float r, float g, float b, float a)
//        : Vec4<float>{ r, g, b, a }
//    {}
//
//    constexpr Color(int c)
//        : Vec4<float>
//    {
//        (float)(c > 0xFFFFFF ? ((c >> 24) & 0xFF) : ((c >> 16) & 0xF)),
//        (float)(c > 0xFFFFFF ? ((c >> 16) & 0xFF) : ((c >> 8) & 0xF)),
//        (float)(c > 0xFFFFFF ? ((c >> 8) & 0xFF) : ((c) & 0xF)),
//        (float)(c > 0xFFFFFF ? ((c) & 0xFF) : 255),
//    }
//    {}
//
//};

using Color = Vec4<float>;

enum struct Align
{
    CENTER, LEFT, RIGHT, TOP, BOTTOM
};

#define M_PI 3.14159265359

#define DEBUG 1
#define FPS 1
#if DEBUG == 1
#define LOG(x) std::cout << x << std::endl
# if FPS == 1
#define MEASURE_FPS static std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); \
static std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); \
end = std::chrono::steady_clock::now(); \
long long diff = \
std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count(); \
float fps = 1000000000.0 / diff; \
LOG("FPS = " << fps); \
begin = std::chrono::steady_clock::now();
# else
#define MEASURE_FPS
# endif
#else
#define LOG(x)
#define MEASURE_FPS
#endif

#define LIMIT_FPS(x) \
static std::chrono::steady_clock::time_point endl = std::chrono::steady_clock::now(); \
static std::chrono::steady_clock::time_point beginl = std::chrono::steady_clock::now(); \
endl = std::chrono::steady_clock::now(); \
long long diffl = \
std::chrono::duration_cast<std::chrono::nanoseconds>(endl - beginl).count(); \
int ns = 1000000000.0 / x; \
int sleepfor = ns - diffl; \
if (sleepfor > 0) \
std::this_thread::sleep_for(std::chrono::nanoseconds(sleepfor)); \
beginl = std::chrono::steady_clock::now();
