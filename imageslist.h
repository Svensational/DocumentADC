#ifndef IMAGESLIST_H
#define IMAGESLIST_H

#include <QAbstractListModel>

class Image;

class ImagesList : public QAbstractListModel {

   Q_OBJECT

public:
   ImagesList(QObject * parent = 0);
   int rowCount(QModelIndex const & parent = QModelIndex()) const;
   QVariant data(QModelIndex const & index, int role) const;

private:
   QList<Image *> list;
};

#endif // IMAGESLIST_H
