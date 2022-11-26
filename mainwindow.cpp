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
            query.exec("SELECT * FROM Course WHERE Major='CPSC'");

            QVBoxLayout* scrollLayout = new QVBoxLayout(this);

            while(query.next()) {
                QCheckBox *checkbox = new QCheckBox(query.value(5).toString() + "-" + query.value(0).toString() + " - " + query.value(2).toString(), this);
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


void MainWindow::on_submitCoursesButton_clicked()
{
    QSqlQuery query_find_course;
        QString dept_name = "'CPSC'";
        QString course_num = "'131'";

        QString squery_find = "SELECT * FROM Course WHERE DName = " + dept_name + " && CNum = " + course_num;

        qDebug() << squery_find;

        query_find_course.prepare(squery_find);

        query_find_course.first();
        query_find_course.exec();
        while(query_find_course.next()){
            qDebug() << query_find_course.value(0).toString();
            qDebug() << query_find_course.value(1).toString();
                    qDebug() << query_find_course.value(2).toString();
                    qDebug() << query_find_course.value(3).toString();
                    qDebug() << query_find_course.value(4).toString();

        }

    QSqlQuery query_where;
        query_where.prepare("CC_ID != ? ,");



    QSqlQuery query_CC;
        query_CC.prepare("SELECT * FROM Course_Core WHERE CC_ID = ?");


    QString query_DName;
    QString query_CNum;
    QString DName;
    QString CName;
    QString CNum;
    QString course;

    int counter = 0;

    QWidget *scrollLayout = ui->scrollAreaWidgetContents;

    QList<QCheckBox *> allBoxes = scrollLayout->findChildren<QCheckBox *>();

    for(QCheckBox *checkbox : allBoxes) {
        if(checkbox->isChecked()) {
            for(auto it : checkbox->text()) {
                if(counter < 4) {
                    DName.append(it);
                }
                else if(counter > 4 && counter < 8) {
                    CName.append(it);
                }
                else { CNum.append(it);
            }
        }
        query_find_course.bindValue(":dept_name", DName);
        query_find_course.bindValue(":course_num", CNum);
        query_find_course.exec();
        qDebug() << query_find_course.value(0).toString();
        qDebug() << query_find_course.value(1).toString();
        qDebug() << query_find_course.value(2).toString();
        qDebug() << query_find_course.value(3).toString();
        qDebug() << query_find_course.value(4).toString();

        DName = "";
        CName = "";
        CNum = "";
//        qDebug() << DName << CName << CNum;
        counter = 0;
    }
}
}
