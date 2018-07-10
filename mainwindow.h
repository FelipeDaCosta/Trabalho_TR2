#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

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

    QString request;
    QString reply;

    QPlainTextEdit* requestEdit;
    QPlainTextEdit* replyEdit;

    bool letItGo;
    bool requestUp;
    bool replyUp;

    void alertChange();

    void setReply(QString reply);
    void setRequest(QString request);

signals:
    void sendNewReply(QString reply);
    void sendNewRequest(QString request);
};

#endif // MAINWINDOW_H
