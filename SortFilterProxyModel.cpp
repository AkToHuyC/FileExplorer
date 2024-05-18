#include "SortFilterProxyModel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {}

//  метод сравнения для сортировки
bool SortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const {
    const FileSystemModel *model = qobject_cast<const FileSystemModel*>(sourceModel());

    if (!model)
        return QSortFilterProxyModel::lessThan(left, right);

    QFileInfo leftFileInfo = model->fileInfo(left);
    QFileInfo rightFileInfo = model->fileInfo(right);

    switch (left.column()) {
    case 0:
        return leftFileInfo.fileName().toLower() < rightFileInfo.fileName().toLower();
    case 1:
        return leftFileInfo.isDir() && !rightFileInfo.isDir();
    case 2:
        return leftFileInfo.size() < rightFileInfo.size();
    case 3:
        return leftFileInfo.lastModified() < rightFileInfo.lastModified();
    default:
        return QSortFilterProxyModel::lessThan(left, right);
    }
}
