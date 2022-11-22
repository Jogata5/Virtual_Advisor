#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBoxMajors->addItem("Computer Science");
    ui->comboBoxMajors->addItem("Psychology");

    ui->comboBoxCatalog->addItem("2019");
    ui->comboBoxCatalog->addItem("2020");




//    this -> setCentralWidget(ui->menuFile);



//    submit = new QPushButton("Submit", this);
//    submit-> setGeometry(10,10,80,30);
//    if(submit->clicked()){

//    }

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_comboBoxMajors_activated(int index)
{
    switch(index){
    case 0:
        break;
    default:
        ui->comboBoxCatalog->show();
    }


}

