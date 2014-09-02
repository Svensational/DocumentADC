#include "savedialog.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QImageWriter>
#include <QLineEdit>
#include <QPushButton>

SaveDialog::SaveDialog(QWidget * parent) :
   QDialog(parent)
{
   QFormLayout * mainLayout = new QFormLayout();
    QHBoxLayout * dirLayout = new QHBoxLayout();
     dirEdit = new QLineEdit();
     dirLayout->addWidget(dirEdit);
     QPushButton * dirButton = new QPushButton("...");
      dirButton->setFixedWidth(dirButton->sizeHint().height());
      connect(dirButton, SIGNAL(clicked()), this, SLOT(chooseDir()));
     dirLayout->addWidget(dirButton);
    mainLayout->addRow(tr("Directory:"), dirLayout);
    suffixBox = new QComboBox();
     foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        suffixBox->addItem(format);
     }
     suffixBox->setCurrentText("png");
    mainLayout->addRow(tr("Format:"), suffixBox);
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Save |
                                                        QDialogButtonBox::Cancel);
     connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
     connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    mainLayout->addRow(buttonBox);
   setLayout(mainLayout);
   setWindowTitle(tr("Save as images"));
}

void SaveDialog::chooseDir() {
   QString dir = QFileDialog::getExistingDirectory(this,
                                                   tr("Choose directory"),
                                                   dirEdit->text());
   if (!dir.isEmpty()) {
      dirEdit->setText(dir);
   }
}

QString SaveDialog::getDir() const {
   return dirEdit->text();
}

QString SaveDialog::getSuffix() const {
   return suffixBox->currentText();
}
