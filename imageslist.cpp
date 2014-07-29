#include "imageslist.h"
#include "image.h"

ImagesList::ImagesList(QObject * parent) :
   QAbstractListModel(parent)
{
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

int ImagesList::rowCount (QModelIndex const & parent) const {
   if (parent.isValid()) {
      return 0;
   }
   else {
      return list.size();
   }
}
