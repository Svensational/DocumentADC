#ifndef IMAGE_H
#define IMAGE_H

#include <QPageLayout>
#include <QPair>
#include <QString>

class QImage;

class Image {

   struct ScanLine {
     uchar const * in;
     uchar * out;
     uint width;
     ScanLine(uchar const * in, uchar * out, uint width);
   };

public:
   Image(QString const & filename);
   ~Image();
   operator bool() const;
   Qt::CheckState getChecked() const;
   QString const & getName() const;
   bool isValid() const;
   void setChecked(Qt::CheckState checkState);
   void setName(QString const & newName);
   QImage const & getTouched();
   QPageLayout::Orientation getOrientation();

   void clear();
   void convertToGrayscale();
   void removeBackground(int kernelSize);
   void windowing(int min, int max);
   void save(); ///< @todo add folder and format

private:
   bool grayscale;
   Qt::CheckState checked;
   QString name;
   QImage * untouched;
   QImage * touched;

   void ensureTouchedExistence();
   static void convertToGrayscaleMT(ScanLine & scanLine);
   static void removeBackgroundGray1MT(QPair<ScanLine, int> & data);
   static void removeBackgroundGray2MT(QPair<ScanLine, QPair<ushort, ushort>> & data);
   static void windowingGrayMT(QPair<ScanLine, QPair<float, float>> & data);
};

#endif // IMAGE_H
