#include <cassert>
#include <iterator>
#include <QFileInfo>
#include <QDir>
#include "utility.h"
#include "FileSystemModel.h"

#include <QDebug>

class Item
{
public:
  enum TYPE{TYPE_FILE, TYPE_DIR, TYPE_LINK};
  //Item(const QString& path, const Item* parent = NULL);
  Item(const QFileInfo& fileInfo, Item* parent = NULL);
  ~Item();

  Item* child(int row) const;
  Item* parent() const;
  int rank(const Item* child) const;
  enum FILE_ATTRIBUTE{FILE_NAME, FILE_TYPE, FILE_SIZE, FILE_ICON, FILE_END};
  QVariant attribute(enum FILE_ATTRIBUTE attribute) const;
  int childCount() const;
  void addChild(Item* child);
  void removeChild(const Item* child);
  void removeChild(const QString& name);

private:
  //const QString name_;
  //const TYPE type_;
  //const QString& path_;
  Item* parent_;
  QVariant attributes_[FILE_END];
  typedef QList<Item*> List;
  List children_;  
};

// Item::Item(const QString& path, const Item* parent)
//   :path_(path),
//    parent_(parent)
// {
//   QFileInfo fileInfo(path);
//   if(fileInfo.isFile() || fileInfo.isSymLink)
//     {
//       attributes_[FILE_NAME] = fileInfo.fileName();
//       attributes_[FILE_TYPE] = TYPE_FILE;
//     }
//   else if(fileInfo.isDir())
//     {
//       attributes_[FILE_NAME] = QDir(path).dirName();
//       attributes_[FILE_TYPE] = TYPE_DIR;
//       const QFileInfoList fileInfoList = fileInfo.dir().entryInfoList(QDir::NotDotAndDotDot);
//       for(QFileInfoList::const_iterator iterator = fileInfoList.begin(); iterator != fileInfoList.end(); ++iterator)
//         {
//         }
//     }
//   attributes_[FILE_SIZE] = FormatFileSize(fileInfo.size());
// }

Item::Item(const QFileInfo& fileInfo, Item* parent)
  :parent_(parent)
{
  if(fileInfo.isFile() || fileInfo.isSymLink())
    {
      attributes_[FILE_NAME] = fileInfo.fileName();
      attributes_[FILE_TYPE] = TYPE_FILE;
    }
  else if(fileInfo.isDir())
    {
      attributes_[FILE_NAME] = fileInfo.dir().dirName();
      attributes_[FILE_TYPE] = TYPE_DIR;
      //const QFileInfoList fileInfoList = QDir(fileInfo.filePath()).entryInfoList(QDir::NoDotAndDotDot);
      const QFileInfoList fileInfoList = QDir(fileInfo.filePath()).entryInfoList();
      qDebug() << fileInfo.size();
      qDebug() << fileInfo.filePath();
      for(QFileInfoList::const_iterator iterator = fileInfoList.begin(); iterator != fileInfoList.end(); ++iterator)
        {
          if(iterator->fileName() != "." && iterator->fileName() != "..")
            {
              Item* child = new Item(*iterator, this);
              children_.push_back(child);
            }
        }
    }
  attributes_[FILE_SIZE] = FormatFileSize(fileInfo.size());
}

Item::~Item()
{
  for(List::const_iterator iterator = children_.begin(); iterator != children_.end(); ++iterator)
    {
      delete *iterator;
    }  
}

Item* Item::child(int row) const
{
  assert(row < children_.size());
  return *(children_.begin() + row);
}

Item* Item::parent() const
{
  return parent_;
}

int Item::rank(const Item* child) const
{
  int childRank = -1;
  for(List::const_iterator iterator = children_.begin(); iterator != children_.end(); ++iterator)
    {
      if(*iterator == child)
        {
          childRank = std::distance(children_.begin(), iterator);
          break;
        }
    }

  return childRank;
}

QVariant Item::attribute(enum FILE_ATTRIBUTE attribute) const
{
  return attributes_[attribute];
}

int Item::childCount() const
{
  return children_.size();
}

void Item::addChild(Item* child)
{
  assert(child != NULL);
  child -> parent_ = this;
  children_.push_back(child);  
}

void Item::removeChild(const Item* child)
{
  assert(child != NULL);
  removeChild(child -> attributes_[FILE_NAME].toString());
}

void Item::removeChild(const QString& name)
{
  for(List::const_iterator iterator = children_.begin(); iterator != children_.end(); ++iterator)
    {
      if((*iterator)->attributes_[FILE_NAME].toString() == name)
        {
          delete *iterator;
          return;
        }      
    }  
}

FileSystemModel::FileSystemModel(QObject* parent)
  :QAbstractItemModel(parent),
   root_(NULL)
{
}

FileSystemModel::FileSystemModel(const QString& rootPath, QObject* parent)
  :QAbstractItemModel(parent),
   root_(NULL)
{
  //setRootPath(rootPath);
}

bool FileSystemModel::setRootPath(const QString& rootPath)
{
  QFileInfo fileInfo(rootPath);
  if(!fileInfo.exists())
    {
      assert("Not a valid path");
      return false;      
    }

  beginResetModel();
  root_ = new Item(fileInfo, NULL);
  endResetModel();
  //reset();

  return true;
}

FileSystemModel::~FileSystemModel()
{
  delete root_;
}

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex& parent) const
{
  if(parent.isValid())
    {
      const Item* parentItem = static_cast<Item*>(parent.internalPointer());
      Item* item = parentItem->child(row);
      return createIndex(row, column, item);
    }
  else
    {
      assert(row == 0);
      return createIndex(row, column, root_);
    }  
}

QModelIndex FileSystemModel::parent(const QModelIndex& index) const
{
  if(index.isValid())
    {
      const Item* item = static_cast<Item*>(index.internalPointer());
      if(item != root_)
        {
          Item* parentItem = item -> parent();
          const Item* grandFatherItem = parentItem -> parent();
          const int row = (parentItem == root_) ? (0) : (grandFatherItem -> rank(parentItem));
          const int column = 0;
          return createIndex(row, column, parentItem);
        }
    }

  return QModelIndex();
}

int FileSystemModel::rowCount(const QModelIndex& parent) const
{
  if(parent.isValid())
    {
      const Item* item = static_cast<Item*>(parent.internalPointer());
      return item -> childCount();
    }
  else
    {
      return 1;
    }
}

int FileSystemModel::columnCount(const QModelIndex& parent) const
{
  return 1;  
}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role)
{
  return QVariant();
}

QVariant FileSystemModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
    return QVariant();

  const Item* item = static_cast<Item*>(index.internalPointer());
  switch(role)
    {
    case Qt::DisplayRole:
      return item -> attribute(Item::FILE_NAME);
    case Qt::CheckStateRole:
      return Qt::Unchecked;
    defaulte:
      return QVariant();
    }

  return QVariant();
}

// bool FileSystemModel::setData(const QModelIndex& index, const QVariant& value, int role)
// {
//   return true;
// }

Qt::ItemFlags FileSystemModel::flags(const QModelIndex& index) const
{
  return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

// bool FileSystemModel::setRootPath(const QString& rootPath)
// {
//   QFileInfo fileInfo(rootPath);
//   if(!fileInfo.exists())
//     {
//       assert("Not a valid path");
//       return false;      
//     }

//   TYPE type;
//   if(fileInfo.isFile())
//     {
//       type = Item::TYPE_FILE;
//     }
//   else if(fileInfo.isDir())
//     {
//       type = Item::TYPE_DIR;      
//     }
//   else if(fileInfo.isSymLink())
//     {
//       type = Item::LINK;            
//     }

//   const QString& name = fileInfo.canonicalFilepath();
//   root_ = new Item(name, type, NULL);
    
//   reset();

//   return true;
// }
