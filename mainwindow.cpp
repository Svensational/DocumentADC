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

MainWindow::MainWindow(QWidget * parent) :
   QMainWindow(parent)
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
                                                     "Select one or more images to open",
                                                     QString(),
                                                     filter);
   if (!files.isEmpty()) {
      imagesList->loadImages(files);
   }
}
