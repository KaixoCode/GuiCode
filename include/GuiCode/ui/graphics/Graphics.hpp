#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/graphics/Shader.hpp"
#include "GuiCode/ui/graphics/Collection.hpp"

#ifdef _DEBUG
#define SHADER(a) ABSOLUTE_SHADER_PATH##a
#define ASSET(a) ABSOLUTE_ASSETS_PATH##a
#define THEME(a) ABSOLUTE_THEMES_PATH##a
#else
#define SHADER(a) "./shaders/"##a
#define ASSET(a) "./assets/"##a
#define THEME(a) "./themes/"##a
#endif

// --------------------------------------------------------------------------
// ----------------------------- Graphics -----------------------------------
// --------------------------------------------------------------------------

namespace Graphics
{
    void SetProjection(const glm::mat4& proj);
    bool WindowFocused();
    void WindowFocused(bool a);
    void CurrentWindow(int a);

    void RunCommands(const ::CommandCollection&);
    
    // --------------------------------------------------------------------------
    // ---------------------------- Textures ------------------------------------
    // --------------------------------------------------------------------------

    struct Image
    {
        unsigned char* data;
        int width, height, channels;
    };

    Image LoadImageFile(const std::string& path);
    void FreeImage(const Image& image);

    struct Texture 
    {
        unsigned int TextureID = 0;

        int Width = -1;
        int Height = -1;

        Texture();
        Texture(HICON a);
        Texture(unsigned int a);
        Texture(const std::string& path);
        void SetTexture(const std::string& path);
        void SetTexture(HICON a);
        operator unsigned int() { return TextureID; }
    };

    // --------------------------------------------------------------------------
    // -------------------------- Text Rendering --------------------------------
    // --------------------------------------------------------------------------

    void LoadFont(int id, int name);
    void LoadFont(int id, int name, unsigned int size);

    void Init();
};

