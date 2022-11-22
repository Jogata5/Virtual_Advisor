#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBoxMajors->addItem("");
    ui->comboBoxMajors->addItem("Computer Science");
    ui->comboBoxMajors->addItem("Psychology");


    ui->comboBoxCatalog->addItem("");
    ui->comboBoxCatalog->addItem("2019");
    ui->comboBoxCatalog->addItem("2020");

    ui->comboBoxCatalog->hide();

    ui->resultsGroupBox->hide();

    ui->errorGroupBox->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}


// Controls dropdown list show/hide
void MainWindow::on_comboBoxMajors_activated(int index)
{
    // Checks index of dropdown list
    switch(index){
    case 0:
        if(ui->comboBoxCatalog->isVisible()) {
            ui->comboBoxCatalog->hide();
        }
        break;
    // If dropdown index is a major, show catalog dropdown
    default:
        if(!ui->comboBoxCatalog->isVisible()){
            ui->comboBoxCatalog->show();
        }
    }
}

// Hides Login once completed
void MainWindow::on_submitButton_clicked()
{
    QWidget *login = ui->loginGroupBox;

    QWidget *results = ui->resultsGroupBox;

    QWidget *error = ui->errorGroupBox;

    name = ui->nameText->text();
    cwid = ui->cwidText->text();

    major = ui->comboBoxMajors->currentText();
    catalog_year = ui->comboBoxCatalog->currentText();

    // Checks if name & cwid not blank
    if(name.trimmed().isEmpty() || cwid.trimmed().isEmpty() || major == "" || catalog_year == ""){
        error->show();
    }

    // Checks if groups are visible, hides/shows respectively
    else if(login->isVisible()){
        login->hide();
        if(!results->isVisible()){
            results->show();
        }
    }
}


void MainWindow::on_errorSubmit_clicked()
{
    QWidget *error = ui->errorGroupBox;

    if(error->isVisible()){
        error->hide();
    }
}

