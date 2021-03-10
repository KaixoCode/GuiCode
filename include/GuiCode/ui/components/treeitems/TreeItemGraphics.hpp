#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/treeitems/TreeItem.hpp"
#include "GuiCode/ui/components/treeitems/TreeItemType.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// ------------------------- TreeItem Graphics ------------------------------
// --------------------------------------------------------------------------

namespace TreeItemGraphics
{
    using namespace Graphics;

    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Simple normal tree item graphics
     */
    struct Normal
    {
        /**
         * Render
         * @param b the <code>TreeItem</code>
         * @param d the <code>CommandCollection</code>
         */
        template<typename TreeItemType>
        static void Render(TreeItem<Normal, TreeItemType>& b, CommandCollection& d) { }

        template<>
        static void Render(TreeItem<Normal, TreeItemType::Normal>& b, CommandCollection& d)
        {
            Color _c2 = Color{ 255, 255, 255, 255 };
            Color _c3 = Color{ 128, 128, 128, 255 };
            Color _c1 = b.Hovering() ? Color{ 255, 255, 255, 13 } : Color{ 0, 0, 0, 0 };
            d.Command<Fill>(_c1);
            d.Command<Quad>(b.Position(), b.Size());

            int _x = b.Position().x + 24;
            int _y = b.Position().y + b.Height() / 2 - 8;
            int _w = 16;
            int _h = 16;
            unsigned int _texture = 0;
            auto& _ext = b.Extension();
            if (_ext == ".wav" || _ext == ".mp3")
                _texture = 0;
            d.Command<TexturedQuad>(_texture, Vec4<int>{_x, _y, _w, _h});

            d.Command<Font>(Gidole14, 14.0f);
            d.Command<Fill>(_c2);
            d.Command<TextAlign>(Align::LEFT, Align::CENTER);
            d.Command<Text>(&b.Name(), Vec2<int>{b.X() + 48, b.Y() + b.Height() / 2});
            d.Command<Fill>(_c3);
            d.Command<TextAlign>(Align::RIGHT, Align::CENTER);
        }

        template<>
        static void Render(TreeItem<Normal, TreeItemType::Folder>& b, CommandCollection& d)
        {
            Color _c2 = Color{ 255, 255, 255, 255 };
            Color _c3 = Color{ 128, 128, 128, 255 };
            Color _c1 = b.Hovering() ? Color{ 255, 255, 255, 13 } : Color{ 0, 0, 0, 0 };
            d.Command<Fill>(_c1);
            d.Command<Quad>(b.X(), b.Y() + b.Height() - b.ActualHeight(), b.Width(), b.ActualHeight());

            int _x = b.X() + 24;
            int _y = b.Y() + b.Height() - b.ActualHeight() / 2 - 8;
            int _w = 16;
            int _h = 16;
            d.Command<TexturedQuad>(0, Vec4<int>{_x, _y, _w, _h});

            d.Command<Font>(Gidole14, 14.0f);
            d.Command<Fill>(_c2);
            d.Command<TextAlign>(Align::LEFT, Align::CENTER);
            d.Command<Text>(&b.Name(), Vec2<int>{b.X() + 48, b.Y() + b.Height() - b.ActualHeight() / 2});
            d.Command<Fill>(_c3);
            _x = b.Position().x + 12;
            _y = b.Position().y + b.Height() - b.ActualHeight() / 2;
            _w = 8;
            _h = 8;
            d.Command<Triangle>(_x, _y, _w, _h, b.Expanded() ? -90.0f : 0.0f);
        }
    };
}