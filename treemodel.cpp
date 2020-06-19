#include "treemodel.h"
#include <QDebug>

TreeModel::TreeModel(const QList<Room> &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new RoomTreeItem({tr("Room"), tr("Summary")});
    setupModelData(data, rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    RoomTreeItem *item = static_cast<RoomTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    RoomTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<RoomTreeItem*>(parent.internalPointer());

    RoomTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    RoomTreeItem *childItem = static_cast<RoomTreeItem*>(index.internalPointer());
    RoomTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    RoomTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<RoomTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<RoomTreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

void TreeModel::setupModelData(const QList<Room> &data, RoomTreeItem *parent)
{
    qDebug() << "setting up tree model.." << endl;
    foreach (const Room r, data) {
        RoomTreeItem *room = new RoomTreeItem({r.getName(), r.getId()}, parent);
        if(r.getUserList().size() > 0) {
            foreach (const User u, r.getUserList()) {
                RoomTreeItem *user = new RoomTreeItem({u.getName(), u.getId()}, room);
                room->appendChild(user);
            }
        }
    }
}
