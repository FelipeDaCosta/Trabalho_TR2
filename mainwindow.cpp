#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    prThread(parent, 8228)
{
    ui->setupUi(this);
    letItGo = false;

    replyEdit = ui->replyEdit;
    requestEdit = ui->requestEdit;
    ui->replyBtn->setEnabled(false);
    ui->requestBtn->setEnabled(false);
    requestUp = false;
    replyUp = false;

    connect(ui->letItGo, &QRadioButton::toggled, [=](bool togg) {
        letItGo = togg;

        std::cout << "TOGG "<<togg << std::endl;
        ui->replyBtn->setEnabled(!letItGo && replyUp);
        ui->requestBtn->setEnabled(!letItGo && requestUp);
    });
    connect(&prThread, &ProxyThread::sendReply, this, &MainWindow::setReply, Qt::QueuedConnection);
    connect(&prThread, &ProxyThread::sendRequest, this, &MainWindow::setRequest, Qt::QueuedConnection);

    connect(this, &MainWindow::sendNewReply, &prThread, &ProxyThread::finishReply, Qt::QueuedConnection);
    connect(this, &MainWindow::sendNewRequest, &prThread, &ProxyThread::finishRequest, Qt::QueuedConnection);


    connect(ui->replyBtn, &QPushButton::clicked, this, [=] (bool) {
        std::cout << "NEW REPLY SENT " << std::endl;
        replyUp = false;
        ui->replyBtn->setEnabled(false);
        emit sendNewReply(replyEdit->toPlainText());
    },
    Qt::QueuedConnection);
    connect(ui->requestBtn, &QPushButton::clicked, this,[=] (bool) {
        std::cout << "NEW REQUEST SENT " << std::endl;
        requestUp = false;
        ui->requestBtn->setEnabled(false);
        emit sendNewRequest(requestEdit->toPlainText());
//        prThread.requestMtx.unlock();
        std::cout << "REQUEST BTN DONE" << std::endl;
    },
    Qt::QueuedConnection);

    prThread.Start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::alertChange()
{
    replyEdit->setPlainText(reply);
    requestEdit->setPlainText(request);
}

void MainWindow::setReply(QString reply)
{
    ui->replyBtn->setEnabled(!letItGo && true);
    this->reply = reply;
    alertChange();
    if (letItGo) {
        emit sendNewReply(replyEdit->toPlainText());
    }
}

void MainWindow::setRequest(QString request)
{
    ui->requestBtn->setEnabled(!letItGo && true);
    this->request = request;
    alertChange();
    if (letItGo) {
        emit sendNewRequest(requestEdit->toPlainText());
    }
}

