#ifndef IMAGE_H
#define IMAGE_H

#include <QString>

class QImage;

class Image {

public:
   Image(QString const & filename);
   ~Image();
   operator bool() const;
   Qt::CheckState getChecked() const;
   QString const & getName() const;
   bool isValid() const;
   void setChecked(Qt::CheckState checkState);
   void setName(QString const & newName);

private:
   Qt::CheckState checked;
   QString name;
   QImage * untouched;
   QImage * touched;

   void ensureTouchedExistence();
};

#endif // IMAGE_H
