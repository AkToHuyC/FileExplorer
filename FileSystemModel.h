#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>
#include <QFileIconProvider>
#include <QVector>

// Класс файловой системы, наследующий QAbstractItemModel
class FileSystemModel : public QAbstractItemModel {
    Q_OBJECT

public:
    FileSystemModel(const QString &rootPath, QObject *parent = nullptr);
    ~FileSystemModel();

    // Методы для получения индексов в строках и колонках
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Метод для установки корневого пути
    void setRootPath(const QString &path);

    // Метод для получения информации о файле
    QFileInfo fileInfo(const QModelIndex &index) const;

private:
    // Структура для представления узла в файловой системе
    struct Node {
        explicit Node(const QFileInfo &fileInfo, Node *parent = nullptr)
            : fileInfo(fileInfo), parent(parent) {}

        QFileInfo fileInfo;
        Node *parent;
        QVector<Node*> children;

        int row() const {
            if (parent)
                return parent->children.indexOf(const_cast<Node*>(this));
            return 0;
        }
    };

    // Вспомогательные методы
    QString fileTypeDescription(const QFileInfo &fileInfo) const;
    QString fileSizeDescription(const QFileInfo &fileInfo) const;
    void populateNode(Node *parentNode);
    void deleteNode(Node *node);
    Node *getNode(const QModelIndex &index) const;

    Node *rootNode = nullptr;
    QFileIconProvider iconProvider;
};

#endif // FILESYSTEMMODEL_H
