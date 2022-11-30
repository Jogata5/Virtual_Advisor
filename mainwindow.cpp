#include <QMainWindow>

#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionMatch>


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

    query.exec("SELECT Major, Major_name FROM Major");
    if (query.isActive()) {
        while(query.next()) {
            if (!major_pairs.contains(qMakePair(query.value(1).toString(),query.value(0).toString()))) {
                ui->comboBoxMajors->addItem(query.value(1).toString());
                major_pairs.append(qMakePair(query.value(1).toString(), query.value(0).toString()));
            }
        }
    }
    query.finish();

    ui->comboBoxCatalog->addItem("");

    query.exec("SELECT * FROM Catalog_years;");
    while (query.next()) {
        ui->comboBoxCatalog->addItem(query.value(0).toString());
        qDebug() << query.value(0).toString();
    }
    query.finish();

    ui->comboBoxCatalog->hide();

    ui->resultsGroupBox->hide();

    ui->errorGroupBox->hide();


}

MainWindow::~MainWindow()
{
    delete ui;
}

bool Check_Cwid(QString cwid) {
    for(auto c : cwid) {
        if (!c.isDigit()) {
            return true;
        }
    }
    return false;
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

    for(const auto& it : major_pairs) {
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
    else if(Check_Cwid(cwid)){
        ui->errorLabel->setText("CWID contains letter(s)");
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

            // Create Course List
            QSqlQuery query;
            QString squery = "SELECT * FROM Course WHERE Major='"+major_code+"' && CatYear='"+catalog_year+"';";
            query.exec(squery);

            QScrollArea* newScrollArea = new QScrollArea(ui->stackedWidget);
//            QWidget* newWidget = new QWidget(newScrollArea);
            QFrame* frame = new QFrame(newScrollArea);
            QVBoxLayout* vBoxLayout = new QVBoxLayout(frame);
//            QFrame* frame = new QFrame(newScrollArea);

            while(query.next()) {
                QString Dname = query.value(5).toString();
                QString Cnum = query.value(0).toString();
                QString Cname = query.value(2).toString();
                QString cc_id = query.value(1).toString();
                QCheckBox *checkbox = new QCheckBox(Dname + "-" + Cnum + "-" + Cname, this);
                vBoxLayout->addWidget(checkbox);

                if (!sections.contains(cc_id)) {
                    sections.append(cc_id);
                }
            }

            sections.sort();

            sections.prepend("All");


            frame->sizeHint();
            newScrollArea->setWidgetResizable(true);
//            frame->setLayout(vBoxLayout);
//            newScrollArea->setWidget(frame);

            ui->stackedWidget->addWidget(newScrollArea);
            qDebug() << ui->stackedWidget->indexOf(newScrollArea);
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(newScrollArea));
            stack_widget_sections.insert("All", ui->stackedWidget->indexOf(newScrollArea));

            for (const auto &section : sections) {

                newScrollArea = new QScrollArea(ui->stackedWidget);
//                newWidget = new QWidget(newScrollArea);
                frame = new QFrame(newScrollArea);
                vBoxLayout = new QVBoxLayout(frame);

//                newWidget->deleteLater();
//                newScrollArea->deleteLater();
//                vBoxLayout->deleteLater();
                ui->comboBoxSection->addItem(section);
                qDebug() << section;
                squery = "SELECT * FROM Course WHERE CC_ID='"+section+"' && Major='"+major_code+"' && CatYear='"+catalog_year+"';";
                query.exec(squery);
                qDebug() << squery;
                if (query.isActive()) {

                    qDebug() << "active";
                    while (query.next()) {
                        QString Dname = query.value(5).toString();
                        QString Cnum = query.value(0).toString();
                        QString Cname = query.value(2).toString();
                        QCheckBox *checkbox = new QCheckBox(Dname + "-" + Cnum + "-" + Cname);
                        qDebug() << Dname << Cnum << Cname;
                        vBoxLayout->addWidget(checkbox);
                    }
                    frame->sizeHint();
                    newScrollArea->setWidgetResizable(true);
//                    frame->setLayout(vBoxLayout);
//                    newScrollArea->setWidget(frame);

                    qDebug() << "adding widget";
                    ui->stackedWidget->addWidget(newScrollArea);
//                    ui->stackedWidget->setCurrentWidget(newScrollArea);
                    qDebug() << ui->stackedWidget->indexOf(newScrollArea);
                    qDebug() << ui->stackedWidget->currentWidget()->layout();
                    qDebug() << ui->stackedWidget->currentWidget()->children();
                    qDebug() << ui->stackedWidget->widget(ui->stackedWidget->indexOf(newScrollArea))->layout();
                    qDebug() << ui->stackedWidget->widget(ui->stackedWidget->indexOf(newScrollArea))->children();
                    if (!stack_widget_sections.contains(section)) {
                        qDebug() << "numbering sections";
                        stack_widget_sections.insert(section, ui->stackedWidget->indexOf(newScrollArea));
                    }
                }
            }


//            QList<QScrollArea*> stackContents = ui->stackedWidget->findChildren<QScrollArea*>();
//            QScrollArea* scroll = stackContents.front();
//            QList<QCheckBox*> allBoxes = scroll->findChildren<QCheckBox*>();
//            std::sort(allBoxes.begin(), allBoxes.end());
//            for (QCheckBox* checkbox : allBoxes) {
//                QString Dname = checkbox->text().section('-',0,0);
//                QString Cnum = checkbox->text().section('-',1,1);
//                qDebug() << Dname << Cnum;
//                squery = "SELECT CC_ID FROM Course WHERE DName='"+Dname+"' && CNum='"+Cnum+"' && Major='"+major+"' && CatYear='"+catalog_year+"';";
//                query.exec(squery);
//                if (query.isActive()) {

//                }
//                if (checkbox->text()->)
//            }

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
    QSqlQuery query;
    QString squery = "";
    QString DName = "";
    QString CName = "";
    QString CNum = "";
    int units = 0;
    int units_req = 0;
    QString cc_id = "";

    int counter = 0;

    QList<QScrollArea*> stackContents = ui->stackedWidget->findChildren<QScrollArea*>();

    for (QScrollArea* scroll : stackContents){
        QList<QCheckBox *> allBoxes = scroll->findChildren<QCheckBox*>();
        for(QCheckBox *checkbox : allBoxes) {
            //        qDebug() << "Looping allBoxes";
            if(checkbox->isChecked()) {
                //            qDebug() << "Checked Box";
                for(const auto it : checkbox->text()) {
                    //                qDebug() << "Looping text";
                    if (it == '-') {
                        counter++;
                    }
                    else if(counter == 0 && it != '-' ) {
                        DName.append(it);
                    }
                    else if(counter == 1 && it != '-') {
                        CNum.append(it);
                    }
                    else { CName.append(it);}
                }
                squery = "SELECT CC_ID, units FROM Course WHERE Major='"+major_code+"' && DName='"+DName+"' && CNum='"+CNum+"';";
                query.exec(squery);
                qDebug() << "executed query1: " << squery;
                if (query.first()) {
                    cc_id = query.value(0).toString();
                    units = query.value(1).toInt();
                    qDebug() << "cc_id: " << cc_id << "\nunits: " << units;
                } else { qDebug() << "query not found";}
                query.finish();

                squery = "SELECT units_required FROM Course_Core WHERE CC_ID = '"+cc_id+"';";
                query.exec(squery);
                qDebug() << "executed query2: " << squery;
                if (query.first()) {
                    units_req = query.value(0).toInt();
                    qDebug() << "units_reg: " << units_req;
                } else { qDebug() << "query not found";}
                query.finish();

                if (core_sections.contains(cc_id)) {
                    qDebug() << "core: " << core_sections.value(cc_id).at(0);
                    int section_units = core_sections.value(cc_id).at(0);
                    int taken_units = section_units + units;
                    core_sections[cc_id].insert(0, taken_units);
                    core_sections[cc_id].insert(1, units_req);
                    qDebug() << "units: " << units << "units_reg: " << units_req << "taken_units" << taken_units;
                    if (taken_units > units_req) {
                        qDebug() << "Section Complete";
                    }
                }
                else {
                    core_sections[cc_id].insert(0, units);
                    core_sections[cc_id].insert(1, units_req);
                }

                DName = "";
                CName = "";
                CNum = "";
                counter = 0;
            }
        }
    }
    //    qDebug() << "Done Looping";

    not_in = "";
//    qDebug() << "not in empty";
//    if (!core_sections.isEmpty()){
//        qDebug() << "!core_sections.isEmpy()";
        for (QHash<QString, QVector<int>>::iterator it = core_sections.begin();it != core_sections.end();it++) {
            qDebug() << it.key() << it.value()[0];
            qDebug() << "units_req:" << units_req;
            if(it.value().at(0) >= it.value().at(1)) {
                not_in.append("'"+it.key()+"',");
            }
        }
//        qDebug() << "Done Hash looping";
//    }

    if (!not_in.isEmpty()) {
        not_in.back() = ' ';
        squery = "SELECT * FROM Course WHERE Major='"+major_code+"' && CC_ID NOT IN ("+not_in+");";
    }
    else {
        squery = "SELECT * FROM Course WHERE Major='"+major_code+"';";

    }
    qDebug() << "not_int: " << not_in;
    query.exec(squery);

    qDebug() << "executed query3: " << squery;

//    QVBoxLayout* scrollLayout = new QVBoxLayout();
//    qDebug() << query.isActive();
//    if (query.isActive()) {
//        while(query.next()) {
//            QCheckBox *checkbox = new QCheckBox(query.value(5).toString() + "-" + query.value(0).toString() + "-" + query.value(2).toString());
//            scrollLayout->addWidget(checkbox);
//        }
//    }
    query.finish();

//    if (ui->scrollAreaWidgetContentsResults->layout()) {
//        qDebug() << "deleting widget";
//        ui->scrollAreaWidgetContentsResults->layout()->removeWidget(ui->scrollAreaWidgetContentsResults);
//        delete ui->scrollAreaWidgetContentsResults->layout();
//    }
//    qDebug() << "new layout";
//    ui->scrollAreaWidgetContentsResults->setLayout(scrollLayout);

    qDebug() << "Clear Hash???";
    if(!core_sections.isEmpty()) {
        qDebug() << "Clearing Hash";
        core_sections.clear();
    }
    qDebug() << "Did not Clear Hash";


}

void MainWindow::on_comboBoxSection_currentTextChanged(const QString &arg1)
{

    if (stack_widget_sections.contains(arg1)) {
//        QString squery = "SELECT * FROM Course WHERE CC_ID='"+arg1+"';";
//        QSqlQuery query;
        qDebug() << "switching sections" << arg1;
        qDebug() << ui->stackedWidget->widget(stack_widget_sections.value(arg1));
        ui->stackedWidget->setCurrentIndex(stack_widget_sections.value(arg1));
//        ui->stackedWidget-->
    }
    else {
        qDebug() << "cannot find section";
    }
}

