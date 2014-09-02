#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;

class SaveDialog : public QDialog {

   Q_OBJECT

public:
   explicit SaveDialog(QWidget * parent = 0);
   QString getDir() const;
   QString getSuffix() const;

private:
   QLineEdit * dirEdit;
   QComboBox * suffixBox;

private slots:
   void chooseDir();
};

#endif // SAVEDIALOG_H
