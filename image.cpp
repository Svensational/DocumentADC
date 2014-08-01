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

void Image::setChecked(Qt::CheckState checkState) {
   checked = checkState;
}

void Image::setName(QString const & newName) {
   name = newName;
}
