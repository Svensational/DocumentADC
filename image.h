#ifndef IMAGE_H
#define IMAGE_H

#include <QString>

class QImage;

class Image {

public:
   Image(QString const & filename);
   QString const & getName() const;

private:
   QString name;
   QImage * untouched;
   QImage * touched;
};

#endif // IMAGE_H
