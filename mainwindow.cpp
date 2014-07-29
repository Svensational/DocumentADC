#include "mainwindow.h"
#include <QAction>
#include <QDockWidget>
#include <QLayout>

MainWindow::MainWindow(QWidget * parent) :
   QMainWindow(parent)
{
   setWindowTitle(tr("Document ADC"));

   createListWidget();
   createDockWidgets();
}

MainWindow::~MainWindow() {
}

void MainWindow::createActions() {
   LoadAction = new QAction(tr("Load images"), this);

   ClearAction = new QAction(tr("Clear list"), this);
}

void MainWindow::createDockWidgets() {
   QDockWidget * imagesDockWidget = new QDockWidget(tr("Images"), this);
   imagesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
   imagesDockWidget->setWidget(listWidget);
   addDockWidget(Qt::LeftDockWidgetArea, imagesDockWidget);
}

void MainWindow::createListWidget() {
   listWidget = new QWidget();
   QVBoxLayout * mainLayout = new QVBoxLayout();
   listWidget->setLayout(mainLayout);
}
