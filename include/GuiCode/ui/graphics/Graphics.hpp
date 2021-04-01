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
    void Scaling(float scale);

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
        Texture(unsigned int a);
        Texture(const std::string& path);
        void SetTexture(const std::string& path);
        operator unsigned int() { return TextureID; }
    };

    struct Textures
    {
        inline static Texture
            FileIcon,
            FolderIcon,
            AudioFileIcon,
            Cross1, Cross2, Cross3,
            Maxi1, Maxi2, Maxi3,
            Maxi4, Maxi5, Maxi6,
            Mini1, Mini2, Mini3,
            Logo;
    };

    // --------------------------------------------------------------------------
    // -------------------------- Text Rendering --------------------------------
    // --------------------------------------------------------------------------
    
    namespace Fonts
    {
        extern int Gidole, Gidole14, Gidole16;
    };

    int LoadFont(const std::string& path);
    int LoadFont(const std::string& path, unsigned int size);

    int StringWidth(const std::string&, int font, int size = -1);
    int StringWidth(const std::string_view&, int font, int size = -1);
    int CharWidth(const char&, int font, int size = -1);

    void Init();

    void DebugOverlay(bool v);
};

