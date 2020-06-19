#include "usertreeitem.h"

UserTreeItem::UserTreeItem(int name, User &u)
    :QStandardItem(name)
{
    this->u = &u;
}

UserTreeItem::~UserTreeItem()
{

}

User *UserTreeItem::getU() const
{
    return u;
}
