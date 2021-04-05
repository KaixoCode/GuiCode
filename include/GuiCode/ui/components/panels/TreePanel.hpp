#pragma once
#include "GuiCode/ui/components/panels/Panel.hpp"

// --------------------------------------------------------------------------
// ----------------------------- Tree Panel ---------------------------------
// --------------------------------------------------------------------------

/**
 * A <code>TreePanel</code> can be used to display a tree of files, using 
 * the <code>TreeItem</code> class.
 */
class TreePanel : public Panel<>
{
public:
    /**
     * Constructor 
     */
    TreePanel();

    /**
     * Sorts the contents of this Panel while remaining the tree hierarchy.
     * @param reverse when true it sorts in reverse alphabetical order
     */
    void SortItems(bool reverse);
};