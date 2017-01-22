/***************************************************************************
 *   Copyright (C) 2017 by Nicolas Carion                                  *
 *   This file is part of Kdenlive. See www.kdenlive.org.                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3 or any later version accepted by the       *
 *   membership of KDE e.V. (or its successor approved  by the membership  *
 *   of KDE e.V.), which shall act as a proxy defined in Section 14 of     *
 *   version 3 of the license.                                             *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include <unordered_map>
#include <unordered_set>


/* @brief This class represents the group hiearchy. This is basically a tree structure
   In this class, we consider that a groupItem is either a clip or a group
*/
class GroupsModel
{
public:
    GroupsModel();

    /* @brief Create a groupItem in the hierarchy. Initially it is not part of a group
       @param id id of the groupItem
    */
    void createGroupItem(int id);

    /* @brief Destruct a groupItem in the hierarchy.
       All its children will become their own roots
       @param id id of the groupitem
    */
    void destructGroupItem(int id);

    /* @brief Get the overall father of a given groupItem
       @param id id of the groupitem
    */
    int getRootId(int id) const;

    /* @brief Returns true if the groupItem has no descendant
       @param id of the groupItem
    */
    bool isLeaf(int id) const;

    /* @brief Returns the id of all the descendant of given item (including item)
       @param id of the groupItem
    */
    std::unordered_set<int> getSubtree(int id) const;

    /* @brief Returns the id of all the leaves in the subtree of the given item
       This should correspond to the ids of the clips, since they should be the only items with no descendants
       @param id of the groupItem
    */
    std::unordered_set<int> getLeaves(int id) const;

    /* @brief change the group of a given item
       @param id of the groupItem
       @param groupId id of the group to assign it to
    */
    void setGroup(int id, int groupId);

    /* @brief Remove an item from all the groups it belongs to.
       @param id of the groupItem
    */
    void removeFromGroup(int id);
private:
    std::unordered_map<int, int> m_upLink; //edges toward parent
    std::unordered_map<int, std::unordered_set<int>> m_downLink; //edges toward children

};