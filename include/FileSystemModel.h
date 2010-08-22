#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QAbstractItemModel>

class Item;

class FileSystemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
  explicit FileSystemModel(QObject* parent = 0);
  explicit FileSystemModel(const QString& rootPath, QObject* parent = 0);
  ~FileSystemModel();
  
public:
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex& index) const;
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  Qt::ItemFlags flags(const QModelIndex& index) const;
  QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole);
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  
public:
  bool setRootPath(const QString& rootPath);
  
private:
  Item* root_;
};
 
#endif
