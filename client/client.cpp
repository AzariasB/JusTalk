#include "client.h"
#include "ui_client.h"

Client::Client(QObject *parent) :
    QObject(parent)
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    serverConnection_ = new Connection(this);
    serverConnection_->requestConnection("127.0.0.1",1234);
}


Client::~Client()
{
    delete serverConnection_;
    //delete ui;
}
