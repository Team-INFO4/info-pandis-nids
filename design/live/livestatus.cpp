#include "livestatus.h"
#include "ui_livestatus.h"

Livestatus::Livestatus(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Livestatus)
{
    ui->setupUi(this);
}

Livestatus::~Livestatus()
{
    delete ui;
}

