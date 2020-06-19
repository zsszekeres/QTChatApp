#ifndef USERTREEITEM_H
#define USERTREEITEM_H

#include "user.h"

#include <QStandardItem>


class UserTreeItem : public QStandardItem
{
public:
    UserTreeItem(int name, User &u);
    ~UserTreeItem();

    User *getU() const;

private:
    User *u;
};

#endif // USERTREEITEM_H
