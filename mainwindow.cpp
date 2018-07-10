#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "proxythread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int portNo = 8228;
//    if(argc >= 2){
//        portNo = atoi(argv[1]);
//    }
//    int sockServer = startProxy(portNo);
//    std::cout << "Aracne iniciado, ouvindo na porta " << portNo << std::endl;
//    runProxy(sockServer);
//    ::close(sockServer);
    ProxyThread th(parent, portNo);
    th.Start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
