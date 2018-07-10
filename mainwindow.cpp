#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListView>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    requestModel = new QStringListModel(requestList, NULL);
//    replyModel = new QStringListModel(replyList, NULL);
//    ui->requestView->setModel(requestModel);
//    ui->replyView->setModel(replyModel);

//    connect(ui->requestView, &QListView::clicked, ui->replyView, &QListView::scrollTo);
//    connect(ui->replyView, &QListView::clicked, ui->requestView, &QListView::scrollTo);

    int portNo = 8228;
//    if(argc >= 2){
//        portNo = atoi(argv[1]);
//    }
//    int sockServer = startProxy(portNo);
//    std::cout << "Aracne iniciado, ouvindo na porta " << portNo << std::endl;
//    runProxy(sockServer);
//    ::close(sockServer);
    connect(&prThread, &ProxyThread::sendReply, [=] (std::string reply) {
        replyList << QString::fromStdString(reply);
        alertChange();
    });
    connect(&prThread, &ProxyThread::sendRequest, [=] (std::string request) {
        requestList << QString::fromStdString(request);
        alertChange();
    });
    prThread.Start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::alertChange()
{
//    requestModel->setStringList(requestList);
//    replyModel->setStringList(replyList);
}

