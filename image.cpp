#include "image.h"
#include <QImage>

Image::Image(QString const & filename) :
   checked(Qt::Checked),
   name(filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1)),
   untouched(new QImage(filename)), touched(nullptr)
{
}

Image::~Image() {
   delete untouched;
   delete touched;
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
