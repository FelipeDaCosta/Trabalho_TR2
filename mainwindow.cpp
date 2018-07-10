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

    hasToRequest = false;
    hasToReply = false;

    connect(replyEdit, &QPlainTextEdit::textChanged, [=] () {
        reply = replyEdit->toPlainText();
    });

    connect(requestEdit, &QPlainTextEdit::textChanged, [=] () {
        request = requestEdit->toPlainText();
    });

    connect(ui->letItGo, &QRadioButton::toggled, [=](bool togg) {
        letItGo = togg;

        std::cout << "TOGG "<<togg << std::endl;
        ui->replyBtn->setEnabled(!letItGo && replyUp);
        ui->requestBtn->setEnabled(!letItGo && requestUp);
        if (hasToRequest) {
            hasToRequest = false;
            emit sendNewRequest(requestEdit->toPlainText());
        }
        if (hasToReply) {
            hasToReply = false;
            emit sendNewReply(replyEdit->toPlainText());

        }
    });
    connect(&prThread, &ProxyThread::sendReply, this, &MainWindow::setReply, Qt::QueuedConnection);
    connect(&prThread, &ProxyThread::sendRequest, this, &MainWindow::setRequest, Qt::QueuedConnection);

    connect(this, &MainWindow::sendNewReply, &prThread, &ProxyThread::finishReply, Qt::QueuedConnection);
    connect(this, &MainWindow::sendNewRequest, &prThread, &ProxyThread::finishRequest, Qt::QueuedConnection);


    connect(ui->replyBtn, &QPushButton::clicked, this, [=] (bool) {
        hasToReply = false;
        std::cout << "NEW REPLY SENT " << std::endl;
        replyUp = false;
        ui->replyBtn->setEnabled(false);
        emit sendNewReply(replyEdit->toPlainText());
    },
    Qt::QueuedConnection);
    connect(ui->requestBtn, &QPushButton::clicked, this,[=] (bool) {
        hasToRequest = true;
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
    hasToReply = true;
    ui->replyBtn->setEnabled(!letItGo && true);
    this->reply = reply;
    alertChange();
    if (letItGo) {
        hasToReply = false;
        emit sendNewReply(replyEdit->toPlainText());
    }
}

void MainWindow::setRequest(QString request)
{
    hasToRequest = true;
    ui->requestBtn->setEnabled(!letItGo && true);
    this->request = request;
    alertChange();
    if (letItGo) {
        hasToRequest = false;
        emit sendNewRequest(requestEdit->toPlainText());
    }
}

