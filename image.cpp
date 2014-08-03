#include "image.h"
#include <QtConcurrent>
#include <QImage>

Image::Image(QString const & filename) :
   grayscale(false), checked(Qt::Checked),
   name(filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1)),
   untouched(new QImage(filename)), touched(nullptr)
{
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

   QList<ScanLine> lineList;
   for (int y=0; y<touched->height(); ++y) {
      lineList << ScanLine(touched->constScanLine(y),
                           result->scanLine(y),
                           touched->width());
   }
   QtConcurrent::blockingMap(lineList, convertToGrayscaleMT);

   delete touched;
   touched = result;
   grayscale = true;
}

void Image::convertToGrayscaleMT(Image::ScanLine & scanLine) {
   QRgb const * in = reinterpret_cast<QRgb const *>(scanLine.in);
   for (int x=0; x<scanLine.width; ++x) {
      scanLine.out[x] = qGray(in[x]);
   }
}

void Image::ensureTouchedExistence() {
   if (!touched) {
      touched = new QImage(*untouched);
      grayscale = touched->format()==QImage::Format_Indexed8 && touched->isGrayscale();
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



Image::ScanLine::ScanLine(uchar const * in, uchar * out, int width) :
   in(in), out(out), width(width)
{
}
