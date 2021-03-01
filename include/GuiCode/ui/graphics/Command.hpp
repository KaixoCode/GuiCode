#pragma once
#include "GuiCode/pch.hpp"

namespace Graphics
{
    /**
     * Command Type.
     */
    enum Type
    {
        Fill, Stroke, StrokeWeight,

        FontSize, Font, TextAlign,

        Clip, ClearClip, Translate, PushMatrix, PopMatrix, Scale, Viewport,

        Text, Quad, TexturedQuad, Ellipse, Triangle,

        FrameBuffer, FrameBufferEnd

    };

    /**
     * Basis for a <code>Command</code>.
     */
    struct CommandBase
    {
        // TODO: wip
        bool operator ==(const CommandBase& b)
        {
            if (b.type != type)
                return false;

            switch (type)
            {
            case Quad: return dimension == b.dimension;
            case Fill: return fill == b.fill;
            case Stroke: return stroke == b.stroke;
            }
        }

        Type type;

        CommandBase(Type type) : type(type) {}

        // Stroke Weight
        CommandBase(Type type, float w) : type(type), weight(w) {}
        CommandBase(Type type, double w) : type(type), weight((float)w) {}
                         
        // Translate     
        CommandBase(Type type, const Vec2<int>& t) : type(type), translate(t) {}
                         
        // Stroke/Fill   
        CommandBase(Type type, float r, float g, float b, float a = 255.0f) : type(type), fill({ r, g, b, a }) {}
        CommandBase(Type type, double r, double g, double b, double a = 255.0) : type(type), fill({ (float)r, (float)g, (float)b, (float)a }) {}
        CommandBase(Type type, const Color& c) : type(type), fill(c) {}
                         
        // Text          
        CommandBase(Type type, const std::string* t, const Vec2<int>& p) : type(type), text(t), position(p) {}
        CommandBase(Type type, const std::string* t, int x, int y) : type(type), text(t), position({ x, y }) {}
        CommandBase(Type type, int t, float s) : type(type), font(t), fontSize(s) {}
                         
        // Quad/Ellipse  
        CommandBase(Type type, int x, int y, int w, int h) : type(type), dimension({ x, y, w, h }), rotation(0) {}
        CommandBase(Type type, const Vec4<int>& d) : type(type), dimension(d), rotation(0) {}
        CommandBase(Type type, const Vec2<int>& d1, const Vec2<int>& d2) : type(type), dimension({ d1.x, d1.y, d2.x, d2.y }), rotation(0) {}
                         
        // Textured Quad 
        CommandBase(Type type, unsigned int t, int x, int y, int w, int h) : type(type), texture(t), textureDimension({ x, y, w, h }) {}
        CommandBase(Type type, unsigned int t, const Vec4<int>& d) : type(type), texture(t), textureDimension(d) {}
                         
        // Triangle      
        CommandBase(Type type, int x, int y, int w, int h, float r) : type(type), dimension({ x, y, w, h }), rotation(r) {}
        CommandBase(Type type, int x, int y, int w, int h, double r) : type(type), dimension({ x, y, w, h }), rotation((float)r) {}
        CommandBase(Type type, const Vec4<int>& d, float r) : type(type), dimension(d), rotation(r) {}
        CommandBase(Type type, const Vec4<int>& d, double r) : type(type), dimension(d), rotation((float)r) {}
                         
        // TextAlign     
        CommandBase(Type type, const Vec2<Align>& a) : type(type), align(a) {}
        CommandBase(Type type, Align a, Align b) : type(type), align({ a, b }) {}

        // Framebuffers
        CommandBase(Type type, unsigned int id, bool refresh) : type(type), id(id), refresh(refresh) {}

        ~CommandBase()
        {}

        union
        {
            // Framebuffer
            struct 
            {
                bool refresh;
                unsigned int id;
            };

            // Fill/Stroke
            union { Color fill, stroke; };

            struct
            {
                int font;
                float fontSize;
            };

            struct // Text
            {
                const std::string* text;
                Vec2<int> position;
            };

            struct // Quad/Ellipse/Triangle
            {
                Vec4<int> dimension;
                float rotation;
            };

            struct
            {
                Vec4<int> textureDimension;
                unsigned int texture;
            };

            struct
            {
                union { Vec2<int> translate, scale; };
            };

            // Strokeweight
            union { float weight, fontSize; };

            // TextAlign
            Vec2<Align> align;
        };
    };

    template<Type T>
    struct Command : public CommandBase
    {
        template<typename ...Args>
        Command(Args ...args)
            : CommandBase(args...)
        {
            type = T;
        };
    };
}
