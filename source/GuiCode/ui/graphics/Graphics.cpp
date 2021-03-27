#include "GuiCode/ui/graphics/Graphics.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

// --------------------------------------------------------------------------
// ----------------------------- Graphics -----------------------------------
// --------------------------------------------------------------------------
namespace Graphics
{
    struct FrameBufferTexture
    {
        GLuint frameBuffer;
        GLuint renderTexture;

        bool ignoreRedraw = false;;

        Vec4<int> dimensions;
    };

    
    int prevShader = -1;

    std::stack<unsigned int> m_FrameBufferStack;
    std::unordered_map<unsigned int, FrameBufferTexture> m_FrameBuffers;
    std::unordered_map<unsigned int, bool> m_FrameBufferDrawn;
    bool m_IgnoreDraws = false;

    std::stack<Vec4<int>> m_ClipStack;

    struct Character
    {
        unsigned int TextureID;
        glm::ivec2   Size;
        glm::ivec2   Bearing;
        unsigned int Advance;
    };

    glm::mat4 m_Projection{ 0 };
    glm::mat4 m_ViewProj{ 0 };
    std::stack<glm::mat4> m_MatrixStack;
    glm::mat4 m_Matrix{ 1.0f };
    int m_CurrentWindowId = -1;
    float m_Scaling = 1;
    bool m_WindowFocused = false;
    Vec2<int> m_Size;

    void m_Translate(const glm::vec2& v) 
    { 
        m_Matrix = glm::translate(m_Matrix, glm::vec3(v.x, v.y, 0)); 
        m_ViewProj = m_Projection * m_Matrix;
    }

    void m_Scale(const glm::vec2& v) 
    { 
        m_Matrix = glm::scale(m_Matrix, glm::vec3(v.x, v.y, 1));
        m_ViewProj = m_Projection * m_Matrix;
    }

    void m_PushMatrix() { m_MatrixStack.push(m_Matrix); }
    void m_PopMatrix() 
    { 
        if (m_MatrixStack.size() > 1) 
        { 
            m_Matrix = m_MatrixStack.top(); 
            m_MatrixStack.pop(); 
            m_ViewProj = m_Projection * m_Matrix;
        } 
    }
    
    void SetProjection(const glm::mat4& proj) 
    { 
        m_Projection = proj; 
        m_ViewProj = m_Projection * m_Matrix;
        //m_PrecalcMvp.clear();
        m_Size.width = 2 / m_Projection[0][0];
        m_Size.height = 2 / m_Projection[1][1];
    }
    
    bool WindowFocused() { return m_WindowFocused; }
    void WindowFocused(bool a) { m_WindowFocused = a; }
    void CurrentWindow(int a) { m_CurrentWindowId = a; }
    void Scaling(float scale) { m_Scaling = scale; }

    void Init()
    {
        Graphics::LoadFont(ASSET("fonts/gidole/Gidole-Regular.otf"), Fonts::Gidole);
        Graphics::LoadFont(ASSET("fonts/gidole/Gidole-Regular.otf"), Fonts::Gidole14, 14);
        Graphics::LoadFont(ASSET("fonts/gidole/Gidole-Regular.otf"), Fonts::Gidole16, 16);

        Textures::FileIcon.SetTexture(ASSET("textures/file.png"));
        Textures::AudioFileIcon.SetTexture(ASSET("textures/audiofile.png"));
        Textures::FolderIcon.SetTexture(ASSET("textures/folder.png"));

        Textures::Logo.SetTexture(ASSET("textures/logosmall.png"));
    }

    // --------------------------------------------------------------------------
    // --------------------------- Basic Shapes ---------------------------------
    // --------------------------------------------------------------------------

    void m_Line(const glm::vec4& dim, float width);
    void m_Quad(const glm::vec4& dim, float rotation);
    void m_TexturedQuad(unsigned int texture, const glm::vec4& dim);
    void m_Ellipse(const glm::vec4& dim, const glm::vec2& a);
    void m_Triangle(const glm::vec4& dim, float rotation);
    void m_Text(const std::string* text, float x, float y);
    void m_FrameBuffer(unsigned int id, bool refresh);
    void m_FrameBufferEnd();

    Color m_Fill{ 1, 1, 1, 1 };
    Color m_Stroke{ 1, 1, 1, 1 };

    
    // Text stuff
    float m_FontSize = 16.0f;
    Fonts m_Font = Fonts::Gidole;
    std::unordered_map<Fonts, std::unordered_map<char, Graphics::Character>> m_Fonts;
    std::unordered_map<Fonts, std::unordered_map<char, int>> m_FontAdvance;
    std::unordered_map<Fonts, int> m_Fontsizes;
    
    Vec2<Align> m_TextAlign = { Align::LEFT, Align::BOTTOM };

    void RunCommands(const ::CommandCollection& d)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        for (auto& i : m_FrameBufferDrawn)
            i.second = false;
        
        for (int i = 0; i < d.Get().size(); i++)
        {
           
            const CommandBase* a = &d.Get()[i];
            if (!m_IgnoreDraws)
            {
                switch (a->type)
                {
                case Fill: m_Fill = a->fill / 255.0; break;
                case Stroke: m_Stroke = a->stroke / 255.0; break;
                case TextAlign: m_TextAlign = a->align; break;
                case FontSize: m_FontSize = a->fontSize; break;
                case Font: m_Font = (Fonts)a->font, m_FontSize = a->fontSize; break;
                case Quad: m_Quad(a->dimension, a->rotation); break;
                case Line: m_Line(a->positions, a->thickness); break;
                case TexturedQuad: m_TexturedQuad(a->texture, a->textureDimension); break;
                case Text: m_Text(a->text, a->position.x, a->position.y); break;
                case Ellipse: m_Ellipse(a->diameters, a->angles); break;
                case Triangle: m_Triangle(a->dimension, a->rotation); break;
                }
            }
            switch (a->type)
            {
            case Clip:
            {
                glEnable(GL_SCISSOR_TEST);
                Vec4<int> clip = {
                    std::ceil((a->dimension.x + m_Matrix[3][0]) / m_Scaling),
                        std::ceil((a->dimension.y + m_Matrix[3][1]) / m_Scaling),
                        std::ceil(a->dimension.z / m_Scaling),
                        std::ceil(a->dimension.w / m_Scaling)
                };
                if (!m_ClipStack.empty())
                    clip = clip.Overlap(m_ClipStack.top());
                m_ClipStack.push(clip);
                glScissor(clip.x, clip.y, clip.width, clip.height); break;
            }
            case PopClip:
            {
                m_ClipStack.pop();
                if (m_ClipStack.size() == 0)
                {
                    glDisable(GL_SCISSOR_TEST);
                } 
                else
                {
                    glEnable(GL_SCISSOR_TEST);
                    Vec4<int> clip2 = m_ClipStack.top();
                    glScissor(clip2.x, clip2.y, clip2.width, clip2.height); break;
                }
            }
            case Viewport: glViewport(
                std::floor(a->dimension.x / m_Scaling),
                std::floor(a->dimension.y / m_Scaling),
                std::floor(a->dimension.z / m_Scaling),
                std::floor(a->dimension.w / m_Scaling)); break;
            case ClearClip: glDisable(GL_SCISSOR_TEST); break;
            case Translate: m_Translate(a->translate); break;
            case Scale: m_Scale(a->scale); break;
            case PushMatrix: m_PushMatrix(); break;
            case PopMatrix: m_PopMatrix(); break;
            case FrameBuffer: m_FrameBuffer(a->id, a->refresh); break;
            case FrameBufferEnd: m_FrameBufferEnd(); break;
            }
        }
    }

    void m_FrameBuffer(unsigned int id, bool refresh)
    {
        if (id == 0)
        {
            // Don't ignore draws when rendering to screen
            m_IgnoreDraws = false;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return;
        }

        // Ignore draw commands if no refresh
        m_IgnoreDraws = !refresh;

        // Make framebuffer if it doesn't exist yet
        auto& _hasfb = m_FrameBuffers.find(id);
        if (_hasfb == m_FrameBuffers.end())
        {
            GLuint _fb = 0;
            glGenFramebuffers(1, &_fb);
            glBindFramebuffer(GL_FRAMEBUFFER, _fb);

            GLuint _rt = 0;
            glGenTextures(1, &_rt);

            // "Bind" the newly created texture : all future texture functions will modify this texture
            glBindTexture(GL_TEXTURE_2D, _rt);

            // Give an empty image to OpenGL ( the last "0" )
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Size.width, m_Size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            // Poor filtering. Needed !
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            // Set "renderedTexture" as our colour attachement #0
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _rt, 0);

            // Set the list of draw buffers.
            GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
            glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
            
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                exit(-1);

            m_FrameBuffers.emplace(id, FrameBufferTexture{ _fb, _rt, false, { 0, 0, m_Size } });
        }

        bool drawn = m_Size.width == -1;
        if (m_Size.width != -1)
        {
            auto& it = m_FrameBufferDrawn.find(id);
            if (it != m_FrameBufferDrawn.end())
                drawn = (*it).second, (*it).second = true;
            else
                m_FrameBufferDrawn.emplace(id, true);

            if (!drawn)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                m_PushMatrix();
                m_Translate(-Vec2<int>{ (int)m_Matrix[3][0], (int)m_Matrix[3][1] });
                if (!m_FrameBufferStack.empty())
                    m_Translate(m_FrameBuffers[m_FrameBufferStack.top()].dimensions.position);
        
                m_TexturedQuad(m_FrameBuffers[id].renderTexture, Vec4<int>{ { 0, 0 }, m_Size });
                m_PopMatrix();
            }
        }
        m_FrameBufferStack.push(id);
        
        FrameBufferTexture& _texture = m_FrameBuffers[id];
        glBindFramebuffer(GL_FRAMEBUFFER, _texture.frameBuffer);
        if (!drawn && refresh)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0, 0, 0, 0);
        }
        if (!(m_Size.width == -1 || m_Size.height == -1) && (m_Size.width != _texture.dimensions.width || m_Size.height != _texture.dimensions.height))
        {
            m_IgnoreDraws = false;
            _texture.dimensions = { {0, 0}, m_Size };
            glBindTexture(GL_TEXTURE_2D, _texture.renderTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Size.width, m_Size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        }

        m_FrameBuffers[id].ignoreRedraw = m_IgnoreDraws;
    }

    void m_FrameBufferEnd()
    {
        m_FrameBufferStack.pop();

        if (!m_FrameBufferStack.empty())
        {
            m_IgnoreDraws = m_FrameBuffers[m_FrameBufferStack.top()].ignoreRedraw;
            glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffers[m_FrameBufferStack.top()].frameBuffer);
        }
        else
        {
            m_IgnoreDraws = false;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    // --------------------------------------------------------------------------
    // --------------------------- Basic Shapes ---------------------------------
    // --------------------------------------------------------------------------

    void m_Line(const glm::vec4& dim, float width)
    {
        //static Shader _shader
        //{
        //    SHADER("QuadVertex.shader"),
        //    SHADER("QuadFragment.shader")
        //};

        static Shader _shader
        {
            // Vertex shader
            "#version 330 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform vec4 dim; "
            "void main() { "
            "    gl_Position = vec4(aPos.x * dim.z + dim.x, aPos.y * dim.w + dim.y, 0.0, 1.0); "
            "}",

            // Fragment shader
            "#version 330 core \n "
            "out vec4 FragColor; \n"
            "uniform vec4 color; \n"
            "uniform vec4 realdim; \n"
            "uniform float width; \n"
            "float minimum_distance(vec2 v, vec2 w, vec2 p) {\n"
            "    // Return minimum distance between line segment vw and point p\n"
            "    float l2 = pow(distance(w, v), 2);  // i.e. |w-v|^2 -  avoid a sqrt\n"
            "    if (l2 == 0.0) return distance(p, v);   // v == w case\n"
            "    // Consider the line extending the segment, parameterized as v + t (w - v).\n"
            "    // We find projection of point p onto the line.\n "
            "    // It falls where t = [(p-v) . (w-v)] / |w-v|^2\n"
            "    // We clamp t from [0,1] to handle points outside the segment vw.\n"
            "    float t = max(0, min(1, dot(p - v, w - v) / l2));\n"
            "    vec2 projection = v + t * (w - v);  // Projection falls on the segment\n"
            "    return distance(p, projection);\n"
            "}\n"
            "void main() { \n"
            "    float dist = minimum_distance(realdim.zw, realdim.xy, gl_FragCoord.xy);\n"
            "    if (dist / width > 0.5) FragColor = vec4(color.rgb, 2 * (1 - (dist / width)) * color.a); \n"
            "    else FragColor = color; \n"
            "} \n"

        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);

        else
        {
            float _vertices[] = {
                0.0f, 0.0f,
                1.0f, 1.0f,
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        if (prevShader != 7)
            _shader.Use();
        prevShader = 7;

        width /= m_Scaling;

        glm::vec4 _tdim = dim;
        float delta_x = _tdim.z - _tdim.x;
        float delta_y = _tdim.w - _tdim.y;
        float angle = std::atan2(delta_y, delta_x);
        float dx = std::cos(angle);
        float dy = std::sin(angle);
        _tdim.x -= dx * width * 0.25;
        _tdim.z += dx * width * 0.25;
        _tdim.y -= dy * width * 0.25;
        _tdim.w += dy * width * 0.25;

        glm::vec4 _dim;
        _dim.x = (_tdim.x + m_Matrix[3][0]) * m_Projection[0][0] + m_Projection[3][0];
        _dim.y = (_tdim.y + m_Matrix[3][1]) * m_Projection[1][1] + m_Projection[3][1];
        _dim.z = (_tdim.z + m_Matrix[3][0]) * m_Projection[0][0] + m_Projection[3][0] - _dim.x;
        _dim.w = (_tdim.w + m_Matrix[3][1]) * m_Projection[1][1] + m_Projection[3][1] - _dim.y;

        glm::vec4 _rdim{ 
            (dim.x + m_Matrix[3][0]) / m_Scaling, 
            (dim.y + m_Matrix[3][1]) / m_Scaling, 
            (dim.z + m_Matrix[3][0]) / m_Scaling, 
            (dim.w + m_Matrix[3][1]) / m_Scaling };

        delta_x = _rdim.z - _rdim.x;
        delta_y = _rdim.w - _rdim.y;
        angle = std::atan2(delta_y, delta_x);
        dx = std::cos(angle);
        dy = std::sin(angle);
        _rdim.x += dx * width * 0.25;
        _rdim.z -= dx * width * 0.25;
        _rdim.y += dy * width * 0.25;
        _rdim.w -= dy * width * 0.25;

        _shader.SetVec4("dim", _dim);
        _shader.SetVec4("realdim", _rdim);
        _shader.SetFloat("width", width * 0.5f);
        _shader.SetVec4("color", m_Fill);
        
        glLineWidth(width);
        glBindVertexArray(_VAO);
        glDrawArrays(GL_LINES, 0, 2);
    }


    void m_Quad(const glm::vec4& dim, float rotation)
    {
        //static Shader _shader
        //{
        //    SHADER("QuadVertex.shader"),
        //    SHADER("QuadFragment.shader")
        //};

        static Shader _shader
        {
            // Vertex shader
            "#version 330 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform mat4 mvp; "
            "void main() { "
            "    gl_Position = mvp * vec4(aPos, 0.0, 1.0); "
            "}", 

            // Fragment shader
            "#version 330 core \n "
            "out vec4 FragColor; "
            "uniform vec4 color; "
            "void main() { "
            "    FragColor = color; "
            "} "
       
        };

        static Shader _shader2
        {
            // Vertex shader
            "#version 330 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform vec4 dim; "
            "void main() { "
            "    gl_Position = vec4(dim.x + aPos.x * dim.z, dim.y + aPos.y * dim.w, 0.0, 1.0); "
            "}",

            // Fragment shader
            "#version 330 core \n "
            "out vec4 FragColor; "
            "uniform vec4 color; "
            "void main() { "
            "    FragColor = color; "
            "} "

        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);
    
        else
        {
            float _vertices[] = { 
                0.0f, 0.0f, 
                1.0f, 0.0f, 
                0.0f, 1.0f, 
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        if (rotation != 0)
        {
            if (prevShader != 6)
                _shader.Use();
            prevShader = 6;

            glm::mat4 _model{ 1.0f };
            _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
            _model = glm::translate(_model, glm::vec3{ dim.z / 2, dim.w / 2, 0 });
            _model = glm::rotate(_model, glm::radians(rotation), glm::vec3{ 0, 0, 1 });
            _model = glm::translate(_model, glm::vec3{ -dim.z/2, -dim.w/2, 0 });
            _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });
            _shader.SetMat4("mvp", m_ViewProj * _model);
            _shader.SetVec4("color", m_Fill);
        }
        else
        {
            if (prevShader != 5)
               _shader2.Use();
            prevShader = 5;
            
            glm::vec4 _dim;
            _dim.x = (dim.x + m_Matrix[3][0]) * m_Projection[0][0] + m_Projection[3][0];
            _dim.y = (dim.y + m_Matrix[3][1]) * m_Projection[1][1] + m_Projection[3][1];
            _dim.z = dim.z * m_Projection[0][0];
            _dim.w = dim.w * m_Projection[1][1];

            _shader2.SetVec4("dim", _dim);
            _shader2.SetVec4("color", m_Fill);
        }

        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void m_TexturedQuad(unsigned int texture, const glm::vec4& dim)
    {
        //static Shader _shader
        //{
        //    SHADER("TexturedQuadVertex.shader"),
        //    SHADER("TexturedQuadFragment.shader")
        //};

        static Shader _shader
        {
            "#version 330 core \n layout(location = 0) in vec2 aPos; layout(location = 1) in vec2 aTexCoord; out vec2 texCoord; uniform mat4 projection; uniform mat4 view; uniform mat4 model; void main() { gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0); texCoord = vec2(aTexCoord.x, aTexCoord.y); } ",
            "#version 330 core \n out vec4 FragColor; in vec2 texCoord; uniform sampler2D theTexture; void main() { FragColor = texture(theTexture, texCoord).rgba; } "
        };
    
        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);
    
        else
        {
            float _vertices[] = {
                // positions   // texCoords
                 1.0f,  1.0f,  1.0f, 1.0f,
                 1.0f,  0.0f,  1.0f, 0.0f,
                 0.0f,  1.0f,  0.0f, 1.0f,
                 1.0f,  0.0f,  1.0f, 0.0f,
                 0.0f,  0.0f,  0.0f, 0.0f,
                 0.0f,  1.0f,  0.0f, 1.0f,
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        if (prevShader != 4)
            _shader.Use();
        prevShader = 4;
        
        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });

        _shader.SetMat4("model", _model);
        _shader.SetMat4("view", m_Matrix);
        _shader.SetMat4("projection", m_Projection);

        _shader.SetInt("theTexture", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void m_Ellipse(const glm::vec4& dim, const glm::vec2& a)
    {
        //static Shader _shader
        //{
        //    SHADER("EllipseVertex.shader"),
        //    SHADER("EllipseFragment.shader")
        //};

        static Shader _shader
        {
            "#version 330 core \n "
            
            "layout(location = 0) in vec2 aPos; "
            
            "uniform mat4 mvp; "
            
            "void main() { "
            "    gl_Position = mvp * vec4(aPos, 0.0, 1.0);"
            "} ",

            "#version 330 core \n "
            "out vec4 FragColor; "
            "uniform vec4 color; "
            "uniform vec2 angles; "
            "uniform vec4 dimensions; "
            "void main() { "
            "    vec2 pos = gl_FragCoord.xy; "
            "    float x = dimensions.x; "
            "    float y = dimensions.y; "
            "    float l = sqrt(pow(x - pos.x, 2) + pow(y - pos.y, 2)); "
            "    float a = acos((pos.x - x)/l); "
            "    if (y > pos.y) a = 6.28318530718-a; "
            "    float astart = 0; "
            "    float aend = angles.y - angles.x;"
            "    if (aend < 0) aend = aend + 6.28318530718; "
            "    float aa = a - angles.x;"
            "    if (aa < 0) aa = aa + 6.28318530718; "
            "    float r = (pow(pos.x - x, 2) / pow(dimensions.z / 2, 2)) + (pow(pos.y - y, 2) / pow(dimensions.w / 2, 2)); "
            "    if (aa > aend) { discard; } "
            "    else if (r > 1) { discard; } "
            "    else if (r > 0.90) { FragColor = vec4(color.rgb, 10 * (1 - r) * color.a); } "
            "    else { FragColor = color; } "
            "} "
        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);

        else
        {
            float _vertices[] = { 
                -.5f, -.5f, 
                .5f, -.5f, 
                -.5f, .5f, 
                .5f, -.5f,
                .5f, .5f,
                -.5f, .5f
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        if (prevShader != 3)
            _shader.Use();
        prevShader = 3;
        
        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });

        _shader.SetMat4("mvp", m_ViewProj * _model);
        _shader.SetVec4("color", m_Fill);

        if (a.x == 0 && a.y == 0)
            _shader.SetVec2("angles", { 0, M_PI * 2 });
        else
            _shader.SetVec2("angles", { std::fmod(a.y + 4.0 * M_PI, 2.0 * M_PI), std::fmod(a.x + 4.0 * M_PI, 2.0 * M_PI) });




        glm::vec4 _dim{ (dim.x + m_Matrix[3][0]) / m_Scaling, (dim.y + m_Matrix[3][1]) / m_Scaling, dim.z / m_Scaling, dim.w / m_Scaling };
        _shader.SetVec4("dimensions", _dim);

        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void m_Triangle(const glm::vec4& dim, float rotation)
    {
        //static Shader _shader
        //{
        //    SHADER("QuadVertex.shader"),
        //    SHADER("QuadFragment.shader")
        //};

        static Shader _shader
        {
            "#version 330 core \n layout(location = 0) in vec2 aPos; uniform mat4 projection; uniform mat4 view; uniform mat4 model; void main() { gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0); }",
            "#version 330 core \n out vec4 FragColor; uniform vec4 color; void main() { FragColor = color; } "
        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);

        else
        {
            float _vertices[] = { -0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        if (prevShader != 2)
           _shader.Use();
        prevShader = 2;

        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });
        if (rotation != 0)
        {
            _model = glm::rotate(_model, glm::radians(rotation), glm::vec3{ 0, 0, 1 });
        }

        _shader.SetMat4("model", _model);
        _shader.SetMat4("view", m_Matrix);
        _shader.SetMat4("projection", m_Projection);
        _shader.SetVec4("color", m_Fill);

        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    // --------------------------------------------------------------------------
    // -------------------------- Text Rendering --------------------------------
    // --------------------------------------------------------------------------

    void m_Text(const std::string* text, float x, float y)
    {
        if (text->empty())
            return;
        

        auto _fafa = m_Font;
        if (m_Scaling < 1)
            _fafa = Graphics::Fonts::Gidole;

        auto& _font = Graphics::m_Fonts[_fafa];

        int _size = m_Fontsizes[_fafa];
        float _scale = 1;
        if (_size != 0)
            _scale = m_FontSize / (float)_size;

        static Shader _shader
        {
            // Vertex shader
            "#version 330 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform vec4 dim; "
            "out vec2 texpos; "
            "void main() { "
            "    gl_Position = vec4(dim.x + aPos.x * dim.z, dim.y + aPos.y * dim.w, 0.0, 1.0); "
            "    texpos = vec2(aPos.x, 1-aPos.y); "
            "}",

            // Fragment shader
            "#version 330 core                                                                                      \n"
            "                                                                                                       \n"
            "out vec4 col;                                                                                          \n"
            "                                                                                                       \n"
            "uniform vec4 color;                                                                                    \n"
            "uniform sampler2DArray Texture;                                                                        \n"
            "                                                                                                       \n"
            "uniform int theTexture;                                                                                \n"
            "in vec2 texpos;                                                                                        \n"
            "                                                                                                       \n"
            "void main() {                                                                                          \n"
            "    float sampled = texture(Texture, vec3(texpos, theTexture)).r;                                      \n"
            "    col.rgb = color.rgb; col.a = color.a * sampled;                                                    \n"
            "}",
        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);

        else
        {

            float _vertices[] = {
                // positions  
                 1.0f,  1.0f,  
                 1.0f,  0.0f,  
                 0.0f,  1.0f,  
                 1.0f,  0.0f,  
                 0.0f,  0.0f,  
                 0.0f,  1.0f,  
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        long _totalWidth = 0;
        long _totalHeight = m_FontSize * 0.7;
        const char* _data = text->data();



        if (m_TextAlign.x == Align::RIGHT || m_TextAlign.x == Align::CENTER)
            for (int i = 0; i < text->size(); i++)
            {
                _totalWidth += (_font[_data[i]].Advance >> 6);
            }


        if (prevShader != 1)
            _shader.Use();
        prevShader = 1;

        _shader.SetVec4("color", m_Fill);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, _font[-1].TextureID);
        _shader.SetInt("Texture", 0);

        glBindVertexArray(_VAO);
        for (int i = 0; i < text->size(); i++)
        {
            Character _ch = _font[_data[i]];

            if (_data[i] != ' ')
            {
                float _xpos = x * m_Matrix[0][0] + _ch.Bearing.x * _scale;
                if (m_TextAlign.x == Align::CENTER)
                    _xpos -= 0.5 * _totalWidth * _scale;
                else if (m_TextAlign.x == Align::RIGHT)
                    _xpos -= _totalWidth * _scale;

                float _ypos = y - (_ch.Size.y - _ch.Bearing.y) * _scale;
                if (m_TextAlign.y == Align::CENTER)
                    _ypos -= 0.5 * _totalHeight;
                else if (m_TextAlign.y == Align::TOP)
                    _ypos -= _totalHeight / 0.7;
           
                if (m_Scaling == 1)
                    _ypos = std::round(_ypos),
                    _xpos = std::round(_xpos);

                glm::vec4 _dim;
                _dim.x = (_xpos + m_Matrix[3][0]) * m_Projection[0][0] + m_Projection[3][0];
                _dim.y = (_ypos + m_Matrix[3][1]) * m_Projection[1][1] + m_Projection[3][1];
                _dim.z = m_FontSize * m_Projection[0][0];
                _dim.w = m_FontSize * m_Projection[1][1];

                _shader.SetVec4("dim", _dim);
                _shader.SetInt("theTexture", _data[i]);

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
            x += (_ch.Advance >> 6) * _scale / m_Matrix[0][0];
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void LoadFont(const std::string& path, Graphics::Fonts name)
    {
        LoadFont(path, name, 48);
    }

    void LoadFont(const std::string& path, Graphics::Fonts name, unsigned int size)
    {
        static FT_Library _ft;
        if (!_ft && FT_Init_FreeType(&_ft))
        {
            LOG("ERROR::FREETYPE: Could not init FreeType Library");
            return;
        }

        FT_Face _face;
        if (FT_New_Face(_ft, path.c_str(), 0, &_face))
        {
            LOG("ERROR::FREETYPE: Failed to load font");
            return;
        }
        else
        {
            Graphics::m_Fonts.insert(
                std::pair<Fonts, std::unordered_map<char, Graphics::Character>>(
                    name, std::unordered_map<char, Graphics::Character>{}));

            FT_Set_Pixel_Sizes(_face, 0, size);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            unsigned int _texture;
            glGenTextures(1, &_texture);
            glBindTexture(GL_TEXTURE_2D_ARRAY, _texture);
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RED, size, size, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
           
            Graphics::Character _character0 = {
                    _texture,
                    glm::ivec2(-1, -1),
                    glm::ivec2(-1, -1),
                    -1
            };

            Graphics::m_Fonts[name].insert(std::pair<char, Graphics::Character>((char)-1, _character0));

            for (unsigned char _c = 0; _c < 128; _c++)
            {
                if (FT_Load_Char(_face, _c, FT_LOAD_RENDER))
                {
                    LOG("ERROR::FREETYTPE: Failed to load Glyph");
                    continue;
                }

                Graphics::Character _character = {
                    _c,
                    glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
                    glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
                    static_cast<unsigned int>(_face->glyph->advance.x)
                };

                int _xpos = _character.Bearing.x;
                int _ypos = _character.Size.y - _character.Bearing.y;

                glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0, 0, size - _character.Size.y, _c,
                    _face->glyph->bitmap.width,
                    _face->glyph->bitmap.rows,
                    1, GL_RED,
                    GL_UNSIGNED_BYTE,
                    _face->glyph->bitmap.buffer
                );

                Graphics::m_Fonts[name].insert(
                    std::pair<char, Graphics::Character>(_c, _character));

                Graphics::m_FontAdvance[name].insert(std::pair<char, int>(_c, _character.Advance));
            }
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
            m_Fontsizes.emplace(name, size);
        }

        FT_Done_Face(_face);
        _ft = {};
        FT_Done_FreeType(_ft);
    }

    // --------------------------------------------------------------------------
    // ---------------------------- Textures ------------------------------------
    // --------------------------------------------------------------------------

    Texture::Texture() 
    {}

    Texture::Texture(unsigned int a)
        : TextureID(a)
    {}

    Texture::Texture(const std::string& path)
    {
        SetTexture(path);
    }

    void Texture::SetTexture(const std::string& path)
    {
        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        //Load the image
        int channels;
        unsigned char* _data = stbi_load(path.c_str(), &Width, &Height, &channels, 0);
        if (_data) {
            if (channels == 4) 
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            LOG("Failed to load texture, path: " << path);
        }

        stbi_image_free(_data);
    } 

    Image LoadImageFile(const std::string& path)
    {
        Image _image;
        stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.
        _image.data = stbi_load(path.c_str(), &_image.width, &_image.height, &_image.channels, 0);
        return _image;
    }

    void FreeImage(const Image& image)
    {
        stbi_image_free(image.data);
    }
}