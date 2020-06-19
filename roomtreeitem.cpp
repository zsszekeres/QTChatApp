#include "roomtreeitem.h"

RoomTreeItem::RoomTreeItem(int name, Room &r)
    :QStandardItem(name)
{
    this->r = &r;
}

RoomTreeItem::~RoomTreeItem()
{

}

Room *RoomTreeItem::getR() const
{
    return r;
}
