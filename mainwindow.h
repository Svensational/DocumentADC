#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListView;
class ImagesList;
class SaveDialog;

class MainWindow : public QMainWindow {

   Q_OBJECT

public:
   MainWindow(QWidget * parent = 0);
   ~MainWindow();

private:
   QAction * loadAction;
   QAction * clearAction;
   QAction * runAction;
   QAction * saveImagesAction;
   QAction * savePDFAction;
   QAction * quitAction;
   QWidget * listWidget;
   QListView * imagesListView;
   ImagesList * imagesList;
   SaveDialog * saveDialog;

   void createActions();
   void createMenues();
   void createListWidget();

private slots:
   void loadImages();
   void saveAsImages();
   void saveAsPDF();
};

#endif // MAINWINDOW_H
