#include "imageslist.h"
#include "image.h"

ImagesList::ImagesList(QObject * parent) :
   QAbstractListModel(parent)
{
}

void ImagesList::clear() {
   if (!list.isEmpty()) {
      beginRemoveRows(QModelIndex(), 0, list.size()-1);
      while (!list.isEmpty()) {
         delete list.takeFirst();
      }
      endRemoveRows();
   }
}

QVariant ImagesList::data(QModelIndex const & index, int role) const {
   if (index.row() >= 0 && index.row() < list.size()) {
      switch (role) {
         case Qt::DisplayRole:
            return list.at(index.row())->getName();
         default:
            return QVariant();
      }
   }
   else {
      return QVariant();
   }
}

Qt::ItemFlags ImagesList::flags (QModelIndex const & index) const {
   if (index.isValid()) {
      return Qt::ItemIsSelectable /*| Qt::ItemIsEditable | Qt::ItemIsUserCheckable*/ | Qt::ItemIsEnabled;
   }
   else {
      return Qt::ItemIsEnabled;
   }
}

QVariant ImagesList::headerData(int section, Qt::Orientation orientation, int role) const {
   if (role != Qt::DisplayRole)
      return QVariant();
   else {
      if (orientation == Qt::Horizontal)
         return QString(tr("Name"));
      else
         return QString(tr("Image %1")).arg(section);
   }
}

void ImagesList::loadImages(QStringList const & filenames) {
   QList<Image *> newImages;
   Image * newImage;
   foreach (QString filename, filenames) {
      newImage = new Image(filename);
      if (newImage->isValid()) {
         newImages << newImage;
      }
      else {
         delete newImage;
      }
   }

   if (!newImages.isEmpty()) {
      beginInsertRows(QModelIndex(), list.size(), list.size()+newImages.size()-1);
      list << newImages;
      endInsertRows();
   }
}

int ImagesList::rowCount (QModelIndex const & parent) const {
   if (parent.isValid()) {
      return 0;
   }
   else {
      return list.size();
   }
}
