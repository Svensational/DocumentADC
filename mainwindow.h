#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListView;
class ImagesList;

class MainWindow : public QMainWindow {

   Q_OBJECT

public:
   MainWindow(QWidget * parent = 0);
   ~MainWindow();

private:
   QAction * loadAction;
   QAction * clearAction;
   QAction * runAction;
   QAction * quitAction;
   QWidget * listWidget;
   QListView * imagesListView;
   ImagesList * imagesList;

   void createActions();
   void createMenues();
   void createListWidget();
   void loadImages();
};

#endif // MAINWINDOW_H
