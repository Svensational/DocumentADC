#include "imageslist.h"
#include <QPainter>
#include <QPdfWriter>
#include <QTime>
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
         case Qt::EditRole:
            return list.at(index.row())->getName();
         case Qt::CheckStateRole:
            return list.at(index.row())->getChecked();
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
      return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
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

void ImagesList::run() {
   QTime t;
   t.start();

   foreach (Image * image, list) {
      if (image->getChecked()) {
         image->clear();
         image->convertToGrayscale();
         image->removeBackground(3);
         image->windowing(32, 255-32);
      }
   }

   qDebug("Time elapsed: %d ms", t.elapsed());
}

void ImagesList::saveAsImages(QString const & dir, QString const & suffix) {
   foreach (Image * image, list) {
      if (image->getChecked()) {
         image->save(dir, suffix);
      }
   }
}

void ImagesList::saveAsPDF(QString const & filename) {
   QPdfWriter pdfWriter(filename);
   pdfWriter.setTitle(filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1));
   pdfWriter.setCreator("Document ADC");
   pdfWriter.setPageSize(QPageSize(QPageSize::A4));
   pdfWriter.setPageMargins(QMarginsF(0.0, 0.0, 0.0, 0.0));
   pdfWriter.setResolution(300);
   QPainter painter;

   bool first = true;
   foreach (Image * image, list) {
      if (image->getChecked()) {
         pdfWriter.setPageOrientation(image->getOrientation());

         if (first) {
            painter.begin(&pdfWriter);
            first = false;
         }
         else {
            pdfWriter.newPage();
         }

         painter.drawImage(pdfWriter.pageLayout().fullRectPixels(300), image->getTouched());
      }
   }

   painter.end();
}

bool ImagesList::setData(QModelIndex const & index, QVariant const & value, int role) {
   if (index.row() >= 0 && index.row() < list.size()) {
      switch (role) {
         case Qt::EditRole:
            list.at(index.row())->setName(value.toString());
            emit dataChanged(index, index);
            return true;
         case Qt::CheckStateRole:
            list.at(index.row())->setChecked(static_cast<Qt::CheckState>(value.toInt()));
            emit dataChanged(index, index);
            return true;
         default:
            return false;
      }
   }
   else {
      return false;
   }
}
