#include "image.h"
#include <QImage>

Image::Image(QString const & filename) :
   grayscale(false), checked(Qt::Checked),
   name(filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1)),
   untouched(new QImage(filename)), touched(nullptr)
{
   grayscale = untouched->isGrayscale() && untouched->format() == QImage::Format_Indexed8;
   // just for testing purpose
   convertToGrayscale();
   save();
}

Image::~Image() {
   delete untouched;
   delete touched;
}

void Image::clear() {
   delete touched;
   touched = nullptr;
}

void Image::convertToGrayscale() {
   ensureTouchedExistence();

   if (grayscale) return;

   QImage * result = new QImage(touched->size(), QImage::Format_Indexed8);
   result->setColorCount(256);
   for (int i=0; i<256; ++i) {
      result->setColor(i, qRgb(i, i, i));
   }

   QRgb const * currentLineIn;
   uchar * currentLineOut;
   for (int y=0; y<touched->height(); ++y) {
      currentLineIn = reinterpret_cast<QRgb const *>(touched->constScanLine(y));
      currentLineOut = result->scanLine(y);
      for (int x=0; x<touched->width(); ++x) {
         currentLineOut[x] = qGray(currentLineIn[x]);
      }
   }

   delete touched;
   touched = result;
   grayscale = true;
}

void Image::ensureTouchedExistence() {
   if (!touched) {
      touched = new QImage(*untouched);
   }
}

Qt::CheckState Image::getChecked() const {
   return checked;
}

QString const & Image::getName() const {
   return name;
}

bool Image::isValid() const {
   return untouched && !untouched->isNull();
}

Image::operator bool() const {
   return untouched && !untouched->isNull();
}

void Image::save() {
   ensureTouchedExistence();

   touched->save(name+".png");
}

void Image::setChecked(Qt::CheckState checkState) {
   checked = checkState;
}

void Image::setName(QString const & newName) {
   name = newName;
}
