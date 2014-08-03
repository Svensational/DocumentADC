#ifndef IMAGE_H
#define IMAGE_H

#include <QString>

class QImage;

class Image {

   struct ScanLine {
     uchar const * in;
     uchar * out;
     int width;
     ScanLine(uchar const * in, uchar * out, int width);
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

   void clear();
   void convertToGrayscale();
   void save();

private:
   bool grayscale;
   Qt::CheckState checked;
   QString name;
   QImage * untouched;
   QImage * touched;

   void ensureTouchedExistence();
   static void convertToGrayscaleMT(ScanLine & scanLine);
};

#endif // IMAGE_H
