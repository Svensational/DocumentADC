#ifndef IMAGESLIST_H
#define IMAGESLIST_H

#include <QAbstractListModel>

class Image;

class ImagesList : public QAbstractListModel {

   Q_OBJECT

public:
   ImagesList(QObject * parent = 0);
   int rowCount(QModelIndex const & parent = QModelIndex()) const;
   Qt::ItemFlags flags(QModelIndex const & index) const;
   QVariant data(QModelIndex const & index, int role) const;
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
   void loadImages(QStringList const & filenames);

public slots:
   void clear();

private:
   QList<Image *> list;
};

#endif // IMAGESLIST_H
