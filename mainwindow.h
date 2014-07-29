#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {

   Q_OBJECT

public:
   MainWindow(QWidget * parent = 0);
   ~MainWindow();

private:
   QAction * LoadAction;
   QAction * ClearAction;
   QWidget * listWidget;

   void createActions();
   void createListWidget();
   void createDockWidgets();
};

#endif // MAINWINDOW_H
