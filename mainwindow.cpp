#include "mainwindow.h"
#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QImageReader>
#include <QLayout>
#include <QListView>
#include <QMenuBar>
#include <QSplitter>
#include "imageslist.h"
#include "savedialog.h"

MainWindow::MainWindow(QWidget * parent) :
   QMainWindow(parent), saveDialog(new SaveDialog(this))
{
   setWindowTitle(tr("Document ADC"));

   imagesList = new ImagesList();

   createActions();
   createMenues();
   createListWidget();

   QSplitter * splitter = new QSplitter();
   splitter->addWidget(listWidget);
   setCentralWidget(splitter);
}

MainWindow::~MainWindow() {
}

void MainWindow::createActions() {
   loadAction = new QAction(tr("&Load images"), this);
   loadAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
   connect(loadAction, &QAction::triggered,
           this, &MainWindow::loadImages);

   clearAction = new QAction(tr("Clear list"), this);
   connect(clearAction, &QAction::triggered,
           imagesList, &ImagesList::clear);

   runAction = new QAction(tr("&Run"), this);
   runAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
   connect(runAction, &QAction::triggered,
           imagesList, &ImagesList::run);

   saveImagesAction = new QAction(tr("Save as images"), this);
   connect(saveImagesAction, &QAction::triggered,
           this, &MainWindow::saveAsImages);

   savePDFAction = new QAction(tr("Save as PDF"), this);
   savePDFAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
   connect(savePDFAction, &QAction::triggered,
           this, &MainWindow::saveAsPDF);

   quitAction = new QAction(tr("&Quit"), this);
   quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
   connect(quitAction, &QAction::triggered,
           this, &MainWindow::close);
}

void MainWindow::createListWidget() {
   listWidget = new QWidget();
   QVBoxLayout * mainLayout = new QVBoxLayout();
    imagesListView = new QListView();
     imagesListView->setModel(imagesList);
    mainLayout->addWidget(imagesListView);
   listWidget->setLayout(mainLayout);
}

void MainWindow::createMenues() {
   QMenu * filesMenu = menuBar()->addMenu(tr("Files"));
   filesMenu->addAction(loadAction);
   filesMenu->addAction(clearAction);
   filesMenu->addSeparator();
   filesMenu->addAction(runAction);
   filesMenu->addSeparator();
   filesMenu->addAction(saveImagesAction);
   filesMenu->addAction(savePDFAction);
   filesMenu->addSeparator();
   filesMenu->addAction(quitAction);
}

void MainWindow::loadImages() {
   // Create a filter for the supported image formats
   QString filter("Images (");
   foreach (QByteArray format, QImageReader::supportedImageFormats()) {
      filter.append("*." + format + ' ');
   }
   filter[filter.length()-1] = ')';

   QStringList files = QFileDialog::getOpenFileNames(this,
                                                     tr("Select one or more images to open"),
                                                     QString(),
                                                     filter);
   if (!files.isEmpty()) {
      imagesList->loadImages(files);
   }
}

void MainWindow::saveAsImages() {
   if (saveDialog->exec()) {
      imagesList->saveAsImages(saveDialog->getDir(),
                               saveDialog->getSuffix());
   }
}

void MainWindow::saveAsPDF() {
   QString filename = QFileDialog::getSaveFileName(this,
                                                   "Save as PDF",
                                                   QString(),
                                                   "PDF (*.pdf)");
   if (!filename.isEmpty()) {
      imagesList->saveAsPDF(filename);
   }
}
