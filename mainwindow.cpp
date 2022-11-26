#include <QMainWindow>

#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>
#include <QVBoxLayout>


#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mDatabase = QSqlDatabase::addDatabase("QMYSQL");
       mDatabase.setHostName("localhost");
       mDatabase.setPort(3306);
       mDatabase.setDatabaseName("VirtualAdvisor");
       mDatabase.setUserName("jogata");
       mDatabase.setPassword("password");
       if (!mDatabase.open()){
           QMessageBox::critical(this, "Error", mDatabase.lastError().text());
           return;
        }

    QSqlQuery query;

    ui->comboBoxMajors->addItem("");
    ui->comboBoxMajors->addItem("Computer Science");
//    ui->comboBoxMajors->addItem("Psychology");


    ui->comboBoxCatalog->addItem("");

    query.exec("SELECT * FROM Catalog_years;");
    while (query.next()) {
        ui->comboBoxCatalog->addItem(query.value(0).toString());
        qDebug() << query.value(0).toString();
    }

    ui->comboBoxCatalog->hide();

    ui->resultsGroupBox->hide();

    ui->errorGroupBox->hide();

    major_pairs.append(qMakePair("Computer Science", "CPSC"));


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

    for(auto it : major_pairs) {
        if(it.first == major) {
            major_code = it.second;
        }
    }

    qDebug() << major_code;
    // Checks if name & cwid not blank
    if(name.trimmed().isEmpty() || cwid.trimmed().isEmpty() || major == "" || catalog_year == ""){
        ui->errorLabel->setText("Please complete all boxes");
        error->show();
    }

    else if(cwid.length() != 9){
        ui->errorLabel->setText("CWID length incorrect");
        error->show();
    }
    // Checks if groups are visible, hides/shows respectively
    else if(login->isVisible()){
        login->hide();
        if(!results->isVisible()){
            ui->resultsNameLabel->setText("Hello " + name + "!");
            ui->resultsCWIDLabel->setText("CWID: " + cwid);
            ui->resultsMajorLabel->setText(major);
            ui->resultsYearLabel->setText(catalog_year);

            QSqlQuery query;
            query.exec("SELECT * FROM Course");

            QVBoxLayout* scrollLayout = new QVBoxLayout(this);

            while(query.next()) {
                qDebug() << query.value(1).toString();
                QCheckBox *checkbox = new QCheckBox(query.value(0).toString() + " - " + query.value(2).toString(), this);
                scrollLayout->addWidget(checkbox);
            }
            ui->scrollAreaWidgetContents->setLayout(scrollLayout);
            results->show();
        }
    }
}

// Controls login error msessage
void MainWindow::on_errorSubmit_clicked()
{
    QWidget *error = ui->errorGroupBox;

    if(error->isVisible()){
        error->hide();
    }
}


