#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QTreeView>
#include <QFileInfo>
#include <QDir>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QVector>
#include <QDebug>
#include <QFileIconProvider>
#include <QIcon>
#include <QScopedPointer>
#include "FileSystemModel.h"
#include "SortFilterProxyModel.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QString rootPath = QDir::currentPath(); // Путь

    // Создание окна
    QWidget *widget = new QWidget;
    widget->setWindowTitle(QObject::tr("File Explorer"));
    widget->setMinimumHeight(400);
    widget->setMinimumWidth(600);
    widget->setContentsMargins(1, 1, 1, 1);

    // Инициализация дерева
    QTreeView *treeView = new QTreeView();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(treeView);

    // Инициализация файловой системы
    FileSystemModel *model = new FileSystemModel(rootPath);

    // Инициализация класса для возможности сортировки
    SortFilterProxyModel *proxyModel = new SortFilterProxyModel;
    proxyModel->setSourceModel(model);

    treeView->setModel(proxyModel);
    treeView->setSortingEnabled(true);

    widget->show();
    return a.exec();
}

#include "main.moc"
