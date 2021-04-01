#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/Component.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu accessories -------------------------------
// --------------------------------------------------------------------------

namespace MenuAccessories
{
    // --------------------------------------------------------------------------
    // ------------------------------ Divider -----------------------------------
    // --------------------------------------------------------------------------

    class Divider : public Component
    {
    public:
        Divider(int size, int stroke, int paddingx, int paddingy)
            : m_Size(size), m_PaddingX(paddingx), m_PaddingY(paddingy), m_Stroke(stroke), Component()
        {
            Size({ size, stroke + paddingy * 2 });
        }

        void Render(CommandCollection&) override;
        bool WithinBounds(const Vec2<int>&) const override { return false; }
        void Color(Color c) { m_Color = c; }

    private:
        ::Color m_Color{ 51, 51, 51, 255};
        int m_Size,
            m_PaddingX,
            m_PaddingY,
            m_Stroke;
    };

    class VerticalDivider : public Component
    {
    public:
        VerticalDivider(int size, int stroke, int paddingx, int paddingy)
            : m_Size(size), m_PaddingX(paddingx), m_PaddingY(paddingy), m_Stroke(stroke), Component()
        {
            Size({ stroke + paddingy * 2, size });
        }

        void Render(CommandCollection&) override;
        bool WithinBounds(const Vec2<int>&) const override { return false; }
        void Color(Color c) { m_Color = c; }

    private:
        ::Color m_Color{ 51, 51, 51, 255 };
        int m_Size,
            m_PaddingX,
            m_PaddingY,
            m_Stroke;
    };
}