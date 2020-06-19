#ifndef TREEITEM_H
#define TREEITEM_H

#include "room.h"

#include <QStandardItem>



class RoomTreeItem : public QStandardItem
{
public:
    RoomTreeItem(int name, Room &r);
    ~RoomTreeItem();

    Room *getR() const;

private:
    Room *r;
};

#endif // TREEITEM_H
