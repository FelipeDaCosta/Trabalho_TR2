#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "proxythread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ProxyThread prThread;
};

#endif // MAINWINDOW_H
