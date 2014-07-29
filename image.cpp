#include "image.h"
#include <QImage>

Image::Image(QString const & filename) :
   name(filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1)),
   untouched(new QImage(filename)), touched(nullptr)
{
}

QString const & Image::getName() const {
   return name;
}
