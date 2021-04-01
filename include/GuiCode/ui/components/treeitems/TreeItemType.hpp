#pragma once
#include "GuiCode/ui/components/panels/TreePanel.hpp"
#include "GuiCode/ui/components/treeitems/TreeItem.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// --------------------------- TreeItem Type --------------------------------
// --------------------------------------------------------------------------

namespace TreeItemType
{
    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Normal tree item.
     */
    class Normal : public TreeItemBase     
    {
    public:
        /**
         * Constructor
         * @param path path to file
         * @param size size
         */
        Normal(const std::filesystem::path &path);
    };

    // --------------------------------------------------------------------------
    // ------------------------------- Folder -----------------------------------
    // --------------------------------------------------------------------------

    /**
     * Folder tree item. Contains a sub-<code>TreePanel</code> for more folder and files.
     */
    class Folder : public TreeItemBase
    {
    public:
        /**
         * Constructor
         * @param path path to file
         * @param size size
         */
        Folder(const std::filesystem::path& path);

        auto Size(const Vec2<int>& size) -> void override { m_Size = size; m_ActualHeight = size.height; }
        auto Width(int w)                -> void override { m_Size = { w, m_Size.height }; }
        auto Height(int h)               -> void override { m_Size = { m_Size.width, h }; m_ActualHeight = h; }

        Vec2<int> Size()   const override { return m_Size; }
        int       Height() const override { return m_Size.height; }
        int       Width()  const override { return m_Size.width; }

        /**
         * Height is slightly adjust, this returns the actual height.
         * @return actual height
         */
        int ActualHeight() const { return m_ActualHeight; }

        /**
         * Is this folder expanded?
         * @return true when folder is expanded
         */
        bool Expanded() const { return m_Expanded; }

        /**
         * Emplace <code>TreeItem</code> to the sub-<code>TreePanel</code>
         * @tparam T type of TreeItem
         * @param ...args arguments for the constructor of <code>T</code>
         */
        template<typename T, typename ...Args>
        T& Emplace(Args&&... args)
        {
            return m_TreePanel->Emplace<T>(std::forward<Args>(args)...);
        }

        void Update(const Vec4<int>& viewport) override;

    private:
        TreePanel* m_TreePanel;

        bool m_Expanded = false,
            m_Expanding = false;
        
        int m_ActualHeight;        

        bool ActuallyWithinBounds(const Vec2<int>& pos) const
        {
            return pos.x >= Position().x && pos.x <= Position().x + Size().width
                && pos.y > Position().y + Size().height - m_ActualHeight && pos.y <= Position().y + Size().height;
        }
    };
}
