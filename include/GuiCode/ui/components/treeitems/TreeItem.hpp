#pragma once
#include "GuiCode/ui/components/panels/Panel.hpp"

// --------------------------------------------------------------------------
// ----------------------------- TreeItem Base ------------------------------
// --------------------------------------------------------------------------

/**
 * Basis for a <code>TreeItem</code>
 */
class TreeItemBase : public Container
{
public:
    TreeItemBase(const std::filesystem::path& path, Vec2<int> size = { 250, 22 })
        : m_Path(path), m_Name(path.filename().string()), 
        m_Extension(m_Path.extension().string()), Container(size)
    {}

    /**
     * Returns true when this <code>TreeItem</code> is a folder.
     * @return true if folder
     */
    bool Folder() const { return m_Folder; }

    /**
     * Returns true if the mouse is hovering over this TreeItem.
     * @return true when mouse over
     */
    virtual bool Hovering() const { return m_Hovering; }

    /**
     * File/Folder name.
     * @return name
     */
    const std::string& Name() const { return m_Name; }

    /**
     * If it's a file it returns the extension as an <code>std::string</code>.
     * @return file extension
     */
    const std::string& Extension() const { return m_Extension; }

    /**
     * Returns the path to the file/folder as an <code>std::filesystem::path</code>.
     * @return path
     */
    const std::filesystem::path& Path() const { return m_Path; }

protected:
    std::filesystem::path m_Path;
    std::string m_Name;
    std::string m_Extension;

    bool m_Enabled = true,
        m_Hovering = false,
        m_Folder = false;
};

// --------------------------------------------------------------------------
// -------------------------------- TreeItem --------------------------------
// --------------------------------------------------------------------------

/**
 * Templated <code>TreeItem</code> class, allows fully customized behaviour and graphics.
 * @tparam TreeItemGraphics a class with a static Render method that takes in the <code>TreeItem</code> and the <code>CommandCollection</code>.
 * @tparam TreeItemType the type of tree item, must be a class that inherits TreeItemBase.
 */
template<typename TreeItemGraphics, typename TreeItemType = TreeItemBase>
struct TreeItem : public TreeItemType
{
    using TreeItemType::TreeItemType;

    void Render(CommandCollection& d) override
    {
        TreeItemType::Render(d);
        TreeItemGraphics::Render(*this, d);
    };
};