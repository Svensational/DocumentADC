#ifndef IMAGE_H
#define IMAGE_H

#include <QString>

class QImage;

class Image {

public:
   Image(QString const & filename);
   operator bool() const;
   QString const & getName() const;
   bool isValid() const;

private:
   QString name;
   QImage * untouched;
   QImage * touched;
};

#endif // IMAGE_H
