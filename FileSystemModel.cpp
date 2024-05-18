#include "FileSystemModel.h"
#include <QDebug>

FileSystemModel::FileSystemModel(const QString &rootPath, QObject *parent)
    : QAbstractItemModel(parent) {
    setRootPath(rootPath);
}

FileSystemModel::~FileSystemModel() {
    deleteNode(rootNode);
}

// Получение индексов в строках и колонках
QModelIndex FileSystemModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Node *parentNode = getNode(parent);
    if (parentNode && row < parentNode->children.size()) {
        Node *childNode = parentNode->children.value(row);
        if (childNode)
            return createIndex(row, column, childNode);
    }
    return QModelIndex();
}

// Получение индекса родителя для построения иерархии
QModelIndex FileSystemModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    Node *childNode = static_cast<Node*>(index.internalPointer());
    Node *parentNode = childNode->parent;

    if (parentNode == rootNode || parentNode == nullptr)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

// Загрузка всех строк
int FileSystemModel::rowCount(const QModelIndex &parent) const {
    Node *parentNode = getNode(parent);
    return parentNode ? parentNode->children.count() : 0;
}

// Определение количества столбцов
int FileSystemModel::columnCount(const QModelIndex &parent) const {
    return 4;
}

// Заполнение колонок размерами файлов и их типами
QVariant FileSystemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    Node *node = static_cast<Node*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return node->fileInfo.fileName();
        case 1:
            return fileTypeDescription(node->fileInfo);
        case 2:
            return fileSizeDescription(node->fileInfo);
        case 3:
            return node->fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
        default:
            return QVariant();
        }
    } else if (role == Qt::DecorationRole && index.column() == 0) {
        return iconProvider.icon(node->fileInfo);
    }

    return QVariant();
}

// Определение типа файла
QString FileSystemModel::fileTypeDescription(const QFileInfo &fileInfo) const {
    if (fileInfo.isDir())
        return tr("Directory");

    QString extension = fileInfo.suffix().toLower();
    if (extension == "txt")
        return tr("Text File");
    else if (extension == "jpg" || extension == "jpeg")
        return tr("JPEG Image");
    else if (extension == "png")
        return tr("PNG Image");
    else
        return tr(".%1 File").arg(extension);
}

// Определение размера файла
QString FileSystemModel::fileSizeDescription(const QFileInfo &fileInfo) const {
    if (fileInfo.isDir())
        return "";

    qint64 size = fileInfo.size();
    if (size < 1024 * 1024)
        return QString::number(size) + " B";
    else {
        double sizeInMB = size / (1024.0 * 1024.0);
        return QString::number(sizeInMB, 'f', 2) + " MB";
    }
}

// Заголовки для колонок
QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Type");
        case 2:
            return tr("Size");
        case 3:
            return tr("Modified Date");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

// Установка пути для конструктора файловой системы
void FileSystemModel::setRootPath(const QString &path) {
    beginResetModel();

    QDir rootDir(path);
    if (!rootDir.exists()) {
        qDebug() << "Path does not exist:" << path;
        deleteNode(rootNode);
        rootNode = nullptr;
        endResetModel();
        return;
    }

    deleteNode(rootNode);

    rootNode = new Node(QFileInfo(rootDir.absolutePath()));
    populateNode(rootNode);

    endResetModel();
}

// Получение информации о файле
QFileInfo FileSystemModel::fileInfo(const QModelIndex &index) const {
    if (index.isValid()) {
        Node *node = static_cast<Node*>(index.internalPointer());
        if (node) {
            return node->fileInfo;
        }
    }
    return QFileInfo();
}

// Рекурсивное заполнение узла Node
void FileSystemModel::populateNode(Node *parentNode) {
    QDir dir(parentNode->fileInfo.absoluteFilePath());
    QFileInfoList fileList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    for (const QFileInfo &fileInfo : fileList) {
        Node *childNode = new Node(fileInfo, parentNode);
        parentNode->children.append(childNode);

        if (fileInfo.isDir()) {
            populateNode(childNode);
        }
    }
}

// Очистка узла Node
void FileSystemModel::deleteNode(Node *node) {
    if (node) {
        for (Node *child : node->children) {
            deleteNode(child);
        }
        delete node;
    }
}

// Получение указателя на объект Node
FileSystemModel::Node* FileSystemModel::getNode(const QModelIndex &index) const {
    if (index.isValid()) {
        return static_cast<Node*>(index.internalPointer());
    }
    return rootNode;
}
