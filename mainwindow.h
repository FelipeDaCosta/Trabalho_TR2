#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

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

    void getRequest(std::string request);
    void getRepply(std::string reply);
private:
    Ui::MainWindow *ui;
    ProxyThread prThread;

    QStringList requestList;
    QStringList replyList;
    QStringList requestShortList;
    QStringList replyShortList;

    QStringListModel *requestModel;
    QStringListModel *replyModel;

    void alertChange();
};

#endif // MAINWINDOW_H
