#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "FileSystemModel.h"

class SortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    SortFilterProxyModel(QObject *parent = nullptr);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // SORTFILTERPROXYMODEL_H
