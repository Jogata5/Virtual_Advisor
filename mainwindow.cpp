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


#include "advisors_info.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect_database("QMYSQL", "localhost", "VirtualAdvisor", "jogata", "password", 3306);

    if (!get_database().open()){
        QMessageBox::critical(this, "Error", get_database().lastError().text());
        return;
    }

    QSqlQuery query;

    ui->comboBoxMajors->addItem("");

    query.exec("SELECT Major, Major_name FROM Major");
    if (query.isActive()) {
        while(query.next()) {
            if (!_major_pairs.contains(qMakePair(query.value(1).toString(),query.value(0).toString()))) {
                ui->comboBoxMajors->addItem(query.value(1).toString());
                _major_pairs.append(qMakePair(query.value(1).toString(), query.value(0).toString()));
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
void MainWindow::connect_database(QString database_type, QString host, QString name, QString user, QString password, int port) {
    _mDatabase = QSqlDatabase::addDatabase(database_type);
    _mDatabase.setHostName(host);
    _mDatabase.setDatabaseName(name);
    _mDatabase.setUserName(user);
    _mDatabase.setPassword(password);
    _mDatabase.setPort(port);

}


void MainWindow::set_catalog_year(QString cat_year) {
   _catalog_year = cat_year;
}
void MainWindow::set_cwid(QString cwid) {
    _cwid = cwid;
}
void MainWindow::set_major(QString major) {
    _major = major;
}
void MainWindow::set_major_code(QString code) {
    _major_code = code;
}
void MainWindow::set_name(QString name) {
    _name = name;
}
void MainWindow::set_not_in(QString not_in) {
    _not_in = not_in;
}

//QHash<QString, QVector<int>> MainWindow::get_core_sections() {return _core_sections;}
//QHash<QString,int> MainWindow::get_stack_widget_sections() {return _stack_widget_sections;}

QSqlDatabase MainWindow::get_database() {return _mDatabase;}

QString MainWindow::get_catalog_year() {return _catalog_year;}
QString MainWindow::get_cwid() {return _cwid;}
QString MainWindow::get_major() {return _major;}
QString MainWindow::get_major_code() {return _major_code;}
QString MainWindow::get_name() {return _name;};


//QVector<QPair<QString, QString>> MainWindow::get_major_pairs() {return _major_pairs;}
//QVector<QString> MainWindow::get_sections() {return _sections;}

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


    set_catalog_year(ui->comboBoxCatalog->currentText());
    set_cwid(ui->cwidText->text());
    set_major(ui->comboBoxMajors->currentText());
    set_name(ui->nameText->text());


    for(const auto& it : _major_pairs) {
        if(it.first == get_major()) {
            set_major_code(it.second);
            qDebug() << get_major_code();
        }
    }


    // Checks if name & cwid not blank
    if(get_name().trimmed().isEmpty() || get_cwid().trimmed().isEmpty() || get_major() == "" || get_catalog_year() == ""){
        ui->errorLabel->setText("Please complete all boxes");
        error->show();
    }

    else if(get_cwid().length() != 9){
        ui->errorLabel->setText("CWID length incorrect");
        error->show();
    }
    else if(Check_Cwid(get_cwid())){
        ui->errorLabel->setText("CWID contains letter(s)");
        error->show();
    }
    // Checks if groups are visible, hides/shows respectively
    else if(login->isVisible()){
        login->hide();
        if(!results->isVisible()){
            ui->resultsNameLabel->setText("Hello " + get_name()+ "!");
            ui->resultsCWIDLabel->setText("CWID: " + get_cwid());
            ui->resultsMajorLabel->setText(get_major());
            ui->resultsYearLabel->setText(get_catalog_year());

            // Create Course List
            QSqlQuery query;
            QString squery = "SELECT * FROM Course WHERE Major='"+get_major_code()+"' && CatYear='"+get_catalog_year()+"';";
            query.exec(squery);

            QScrollArea* newScrollArea = new QScrollArea(ui->stackedWidget);
//            QWidget* newWidget = new QWidget(newScrollArea);
            QFrame* frame = new QFrame(newScrollArea);
            QVBoxLayout* vBoxLayout = new QVBoxLayout(frame);
//            QFrame* frame = new QFrame(newScrollArea);

            while(query.next()) {
//                QString Dname = query.value(5).toString();
//                QString Cnum = query.value(0).toString();
//                QString Cname = query.value(2).toString();
                QString cc_id = query.value(1).toString();
//                QCheckBox *checkbox = new QCheckBox(Dname + "-" + Cnum + "-" + Cname, this);
//                vBoxLayout->addWidget(checkbox);

                if (!_sections.contains(cc_id)) {
                    _sections.append(cc_id);
                }
            }

            _sections.sort();

//            sections.prepend("All");


            frame->sizeHint();
            newScrollArea->setWidgetResizable(true);
//            frame->setLayout(vBoxLayout);
//            newScrollArea->setWidget(frame);

            ui->stackedWidget->addWidget(newScrollArea);
            qDebug() << ui->stackedWidget->indexOf(newScrollArea);
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(newScrollArea));
//            stack_widget_sections.insert("All", ui->stackedWidget->indexOf(newScrollArea));

            for (const auto &section : _sections) {

                newScrollArea = new QScrollArea(ui->stackedWidget);
//                newWidget = new QWidget(newScrollArea);
                frame = new QFrame(newScrollArea);
                vBoxLayout = new QVBoxLayout(frame);

//                newWidget->deleteLater();
//                newScrollArea->deleteLater();
//                vBoxLayout->deleteLater();
                ui->comboBoxSection->addItem(section);
                qDebug() << section;
                squery = "SELECT * FROM Course WHERE CC_ID='"+section+"' && Major='"+get_major_code()+"' && CatYear='"+get_catalog_year()+"';";
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
                    if (!_stack_widget_sections.contains(section)) {
                        qDebug() << "numbering sections";
                        _stack_widget_sections.insert(section, ui->stackedWidget->indexOf(newScrollArea));
                    }
                }
            }
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
    QString Dname = "";
    QString Cname = "";
    QString Cnum = "";
    int units = 0;
    int units_req = 0;
    QString cc_id = "";


    QList<QScrollArea*> stackContents = ui->stackedWidget->findChildren<QScrollArea*>();

    for (QScrollArea* scroll : stackContents){
        QList<QCheckBox *> allBoxes = scroll->findChildren<QCheckBox*>();
        for(QCheckBox *checkbox : allBoxes) {
                    qDebug() << "Looping allBoxes";
            if(checkbox->isChecked()) {
                            qDebug() << "Checked Box";
                Dname = checkbox->text().section('-',0,0);
                Cnum = checkbox->text().section('-',1,1);
                Cname = checkbox->text().section('-',2,2);
                squery = "SELECT CC_ID, units FROM Course WHERE Major='"+get_major_code()+"' && DName='"+Dname+"' && CNum='"+Cnum+"';";
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

                if (_core_sections.contains(cc_id)) {
                    qDebug() << "core: " << _core_sections.value(cc_id).at(0);
                    int section_units = _core_sections.value(cc_id).at(0);
                    int taken_units = section_units + units;
                    _core_sections[cc_id].insert(0, taken_units);
                    _core_sections[cc_id].insert(1, units_req);
                    qDebug() << "units: " << units << "units_reg: " << units_req << "taken_units" << taken_units;
                    if (taken_units > units_req) {
                        qDebug() << "Section Complete";
                    }
                }
                else {
                    _core_sections[cc_id].insert(0, units);
                    _core_sections[cc_id].insert(1, units_req);
                }

                Dname = "";
                Cname = "";
                Cnum = "";
            }
        }
    }
    //    qDebug() << "Done Looping";

    set_not_in("");
//    qDebug() << "not in empty";
//    if (!core_sections.isEmpty()){
//        qDebug() << "!core_sections.isEmpy()";
        for (QHash<QString, QVector<int>>::iterator it = _core_sections.begin();it != _core_sections.end();it++) {
            qDebug() << it.key() << it.value()[0];
            qDebug() << "units_req:" << units_req;
            if(it.value().at(0) >= it.value().at(1)) {
                _not_in.append("'"+it.key()+"',");
            }
        }
//        qDebug() << "Done Hash looping";
//    }

    if (!_not_in.isEmpty()) {
        _not_in.back() = ' ';
        squery = "SELECT * FROM Course WHERE Major='"+get_major_code()+"' && CC_ID NOT IN ("+_not_in+");";
    }
    else {
        squery = "SELECT * FROM Course WHERE Major='"+get_major_code()+"';";

    }
    qDebug() << "not_int: " << _not_in;
    query.exec(squery);

    qDebug() << "executed query3: " << squery;

    if (query.isActive()) {

        QScrollArea* newScrollArea = new QScrollArea(ui->scrollAreaWidgetContentsResults);
//            QWidget* newWidget = new QWidget(newScrollArea);
        QFrame* frame = new QFrame(newScrollArea);
        QVBoxLayout* vBoxLayout = new QVBoxLayout(frame);
//            QFrame* frame = new QFrame(newScrollArea);

        while(query.next()) {
            QString Dname = query.value(5).toString();
            QString Cnum = query.value(0).toString();
            QString Cname = query.value(2).toString();
//            QString cc_id = query.value(1).toString();
            QCheckBox *checkbox = new QCheckBox(Dname + "-" + Cnum + "-" + Cname, this);
            vBoxLayout->addWidget(checkbox);
        }

        if (ui->scrollAreaWidgetContentsResults->layout()) {
            delete ui->scrollAreaWidgetContentsResults->layout();
            ui->scrollAreaWidgetContentsResults->setLayout(vBoxLayout);
        }
        else {
            ui->scrollAreaWidgetContentsResults->setLayout((vBoxLayout));
        }
    }

    query.finish();

    qDebug() << "Clear Hash???";
    if(!_core_sections.isEmpty()) {
        qDebug() << "Clearing Hash";
        _core_sections.clear();
    }
    qDebug() << "Did not Clear Hash";


}

void MainWindow::on_comboBoxSection_currentTextChanged(const QString &arg1)
{

    if (_stack_widget_sections.contains(arg1)) {
//        QString squery = "SELECT * FROM Course WHERE CC_ID='"+arg1+"';";
//        QSqlQuery query;
        qDebug() << "switching sections" << arg1;
        qDebug() << ui->stackedWidget->widget(_stack_widget_sections.value(arg1));
        ui->stackedWidget->setCurrentIndex(_stack_widget_sections.value(arg1));
//        ui->stackedWidget-->
    }
    else {
        qDebug() << "cannot find section";
    }
}






//void MainWindow::on_backButton_clicked()
//{
//    ui->resultsGroupBox->hide();

//    _core_sections.clear();
//    _stack_widget_sections.clear();

//    _major_pairs.clear();
//    _sections.clear();

//    _catalog_year = "";
//    _cwid = "";
//    _major = "";
//    _major_code = "";
//    _name = "";
//    _not_in = "";

//    for (int i = ui->stackedWidget->count();i>=0;--i) {
//        QWidget* widget = ui->stackedWidget->widget(i);
//        ui->stackedWidget->removeWidget(widget);
//        widget->deleteLater();
//    }

//    ui->loginGroupBox->show();
//}


void MainWindow::on_advisorButton_clicked()
{


    contact_ui = new Advisors_info(this);

    contact_ui->show_advisor(_major_code);

    contact_ui->show();
}

