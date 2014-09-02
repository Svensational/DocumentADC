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
   for (uint x=0u; x<scanLine.width; ++x) {
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

QPageLayout::Orientation Image::getOrientation() {
   ensureTouchedExistence();

   if (touched->width() > touched->height()) {
      return QPageLayout::Landscape;
   }
   else {
      return QPageLayout::Portrait;
   }
}

QImage const & Image::getTouched() {
   ensureTouchedExistence();

   return *touched;
}

bool Image::isValid() const {
   return untouched && !untouched->isNull();
}

Image::operator bool() const {
   return untouched && !untouched->isNull();
}

void Image::removeBackground(int kernelSize) {
   ensureTouchedExistence();

   QImage * temp = new QImage(touched->size(), touched->format());
   if (grayscale) {
      temp->setColorCount(256);
      for (int i=0; i<256; ++i) {
         temp->setColor(i, qRgb(i, i, i));
      }
   }

   // Pass 1
   QList<QPair<ScanLine, int>> lineList1;
   for (int y=0; y<touched->height(); ++y) {
      lineList1 << QPair<ScanLine, int>(ScanLine(touched->constScanLine(y),
                                                 temp->scanLine(y),
                                                 touched->width()),
                                        kernelSize);
   }
   if (grayscale) {
      QtConcurrent::blockingMap(lineList1, removeBackgroundGray1MT);
   }
   lineList1.clear();

   // Pass 2
   QList<QPair<ScanLine, QPair<ushort, ushort>>> lineList2;
   for (int x=0; x<touched->width(); ++x) {
      lineList2 << QPair<ScanLine, QPair<ushort, ushort>>(ScanLine(temp->constScanLine(0)+x,
                                                                   touched->scanLine(0)+x,
                                                                   touched->width()),
                                                          QPair<ushort, ushort>(kernelSize,
                                                                                touched->height()));
   }
   if (grayscale) {
      QtConcurrent::blockingMap(lineList2, removeBackgroundGray2MT);
   }
   lineList2.clear();
   delete temp;
}

void Image::removeBackgroundGray1MT(QPair<ScanLine, int> & data) {
   uchar max = 0u;
   // half kernel size
   const uint n = data.second << 1;
   for (uint x=0u; x<data.first.width; ++x) {
      // small optimization possible, but not sure if practical for typical kernel sizes
      if (x<n+1u || data.first.in[x-n-1u]==max) {
         // search whole neighborhood for max
         max = 0u;
         for (uint i=qMax(x-n, 0u); i<qMin(x+n+1u, data.first.width); ++i) {
            max = qMax(max, data.first.in[i]);
         }
      }
      else {
         // just compare new pixel with last max
         if (x+n < data.first.width) {
            max = qMax(max, data.first.in[x+n]);
         }
      }
      data.first.out[x] = max;
   }
}

void Image::removeBackgroundGray2MT(QPair<ScanLine, QPair<ushort, ushort>> & data) {
   uchar max = 0u;
   // half kernel size
   const ushort n = data.second.first << 1;
   // bytes per line (data is aligned on a 32-bit boundary)
   const ushort bpl = (data.first.width << 2) >> 2;
   for (uint y=0u; y<data.second.second; ++y) {
      if (y<n+1u || *(data.first.in+(y-n-1u)*bpl)==max) {
         max = 0u;
         for (uint i=qMax(y-n, 0u); i<qMin(y+n+1u, (uint)data.second.second); ++i) {
            max = qMax(max, *(data.first.in+i*bpl));
         }
      }
      else {
         if (y+n < data.second.second) {
            max = qMax(max, *(data.first.in+(y+n)*bpl));
         }
      }
      // divide original data by the background value
      *(data.first.out+y*bpl) /= max/255.0;
   }
}

void Image::save(const QString & dir, const QString & suffix) {
   ensureTouchedExistence();

   touched->save(dir+'/'+name+'.'+suffix);
}

void Image::setChecked(Qt::CheckState checkState) {
   checked = checkState;
}

void Image::setName(QString const & newName) {
   name = newName;
}

void Image::windowing(int min, int max) {
   ensureTouchedExistence();

   const float a = 255.0/float(max-min);
   const float b = -a*min;

   QList<QPair<ScanLine, QPair<float, float>>> lineList;
   for (int y=0; y<touched->height(); ++y) {
      lineList << QPair<ScanLine, QPair<float, float>>(ScanLine(touched->constScanLine(y),
                                                                touched->scanLine(y),
                                                                touched->width()),
                                                       QPair<float, float>(a, b));
   }
   if (grayscale) {
      QtConcurrent::blockingMap(lineList, windowingGrayMT);
   }
   lineList.clear();
}

void Image::windowingGrayMT(QPair<ScanLine, QPair<float, float>> & data) {
   for (uint x=0u; x<data.first.width; ++x) {
      data.first.out[x] = uchar(qBound(0.0f,
                                       data.second.first * data.first.in[x] + data.second.second + 0.5f,
                                       255.0f));
   }
}



Image::ScanLine::ScanLine(uchar const * in, uchar * out, uint width) :
   in(in), out(out), width(width)
{
}
