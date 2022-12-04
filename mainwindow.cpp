//    This program is called Virtual Advisor and aims to provide CSUF Students with
//    an easier method to figure out what classes they should take next. The
//    user will input their information and pick the classes that they have completed.
//    The program will output a list of classes the user can take next.

//    Creators:
//      Julian Ogata    - jogata@csu.fullerton.edu
//      Dilhan Franco   - dilhanfranco@csu.fullerton.edu
//      Parth Sharma    - parthsharma@csu.fullerton.edu

//    Copyright (C) 2022  Julian Ogata, Dilhan Franco, Parth Sharma

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <QMainWindow>

// SQL
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlQuery>

// Widgets and Objects
#include <QCheckBox>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QVBoxLayout>

// h files
#include "advisors_info.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Sets up the MainWindow
    ui->setupUi(this);

    // Connects to the SQL Database
    connect_database("QMYSQL", "localhost", "VirtualAdvisor", "jogata", "password", 3306);

    // If the database is not opened, throw error
    if (!get_database().open()){
        QMessageBox::critical(this, "Error", get_database().lastError().text());
        return;
    }

    // Initialize query value
    QSqlQuery query;

    // Adds a blank item to the Major combobox
    ui->comboBoxMajors->addItem("");

    // Executes query and iterates through results. Inputs items into the combobox
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

    // Adds a blank item to the Catalog combobox
    ui->comboBoxCatalog->addItem("");

    // Executes query and iterates through results. Inputs items into the combobox
    query.exec("SELECT * FROM Catalog_years;");
    while (query.next()) {
        ui->comboBoxCatalog->addItem(query.value(0).toString());
    }
    query.finish();

    // hides Groupboxs and catalog combo box
    ui->comboBoxCatalog->hide();
    ui->errorGroupBox->hide();
    ui->resultsGroupBox->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Method to connect to the datababse
void MainWindow::connect_database(QString database_type, QString host, QString name, QString user, QString password, int port) {
    _mDatabase = QSqlDatabase::addDatabase(database_type);
    _mDatabase.setHostName(host);
    _mDatabase.setDatabaseName(name);
    _mDatabase.setUserName(user);
    _mDatabase.setPassword(password);
    _mDatabase.setPort(port);

}

// Setters
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

// Getters
QSqlDatabase MainWindow::get_database() {return _mDatabase;}
QString MainWindow::get_catalog_year() {return _catalog_year;}
QString MainWindow::get_cwid() {return _cwid;}
QString MainWindow::get_major() {return _major;}
QString MainWindow::get_major_code() {return _major_code;}
QString MainWindow::get_name() {return _name;};

// Method to check CWID is composed of digits
bool Check_Cwid(QString cwid) {
    // iterate through cwid and check each character
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
    // Widgets
    QWidget *error = ui->errorGroupBox;
    QWidget *login = ui->loginGroupBox;
    QWidget *results = ui->resultsGroupBox;

    // Sets values to from user input into private values
    set_catalog_year(ui->comboBoxCatalog->currentText());
    set_cwid(ui->cwidText->text());
    set_major(ui->comboBoxMajors->currentText());
    set_name(ui->nameText->text());

    // iterate through the vector of pairs containing the major name and the major abreviation (ex: computer science, CPSC)
    for(const auto& it : _major_pairs) {
        if(it.first == get_major()) {
            set_major_code(it.second);
        }
    }


    // Checks if name & cwid not blank
    if(get_name().trimmed().isEmpty() || get_cwid().trimmed().isEmpty() || get_major() == "" || get_catalog_year() == ""){
        ui->errorLabel->setText("Please complete all boxes");
        error->show();
    }
    // Checks if cwid is the corect length
    else if(get_cwid().length() != 9){
        ui->errorLabel->setText("CWID length incorrect");
        error->show();
    }
    // Checks if the cwid is made completely of digits
    else if(Check_Cwid(get_cwid())){
        ui->errorLabel->setText("CWID contains letter(s)");
        error->show();
    }
    // Checks if groups are visible, hides/shows respectively
    else if(login->isVisible()){
        // Hides the login window
        login->hide();
        // Checks if the results window is visible
        if(!results->isVisible()){
            // Sets values to the labels in the results window
            ui->resultsNameLabel->setText("Hello " + get_name()+ "!");
            ui->resultsCWIDLabel->setText("CWID: " + get_cwid());
            ui->resultsMajorLabel->setText(get_major());
            ui->resultsYearLabel->setText(get_catalog_year());

            // Create Course List
            QSqlQuery query;
            QString squery = "SELECT * FROM Course WHERE Major='"+get_major_code()+"' && CatYear='"+get_catalog_year()+"';";
            query.exec(squery);

            // Creates the widgets necessary for creating a scroll area containing query results
            QScrollArea* newScrollArea = new QScrollArea(ui->stackedWidget);
            QFrame* frame = new QFrame(newScrollArea);
            QVBoxLayout* vBoxLayout = new QVBoxLayout(frame);

            // Iterates through query results
            while(query.next()) {
                // Sets cc_id to query values
                QString cc_id = query.value(1).toString();
                // Appends sections for section combobox
                if (!_sections.contains(cc_id)) {
                    _sections.append(cc_id);
                }
            }

            // Sorts sections
            _sections.sort();

            // Gives the frame a hint on what size it should be
            frame->sizeHint();
            // Sets the scrollarea to be resizable
            newScrollArea->setWidgetResizable(true);

            // Adds the scroll area widget to the stack widget
            ui->stackedWidget->addWidget(newScrollArea);
            // Sets the index of the new scroll area
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(newScrollArea));

            // Iterates through the sections and adds the proper courses to those sections
            for (const auto &section : _sections) {
                newScrollArea = new QScrollArea(ui->stackedWidget);
                frame = new QFrame(newScrollArea);
                vBoxLayout = new QVBoxLayout(frame);

                ui->comboBoxSection->addItem(section);
                squery = "SELECT * FROM Course WHERE CC_ID='"+section+"' && Major='"+get_major_code()+"' && CatYear='"+get_catalog_year()+"';";
                query.exec(squery);
                if (query.isActive()) {
                    while (query.next()) {
                        QString Dname = query.value(5).toString();
                        QString Cnum = query.value(0).toString();
                        QString Cname = query.value(2).toString();
                        QCheckBox *checkbox = new QCheckBox(Dname + "-" + Cnum + "-" + Cname);
                        vBoxLayout->addWidget(checkbox);
                    }
                    newScrollArea->setWidgetResizable(true);

                    ui->stackedWidget->addWidget(newScrollArea);

                    if (!_stack_widget_sections.contains(section)) {
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

// Controls what occurs when the submit button is clicked
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

    // Creates a list of QScrollAreas from the stackedWidget
    QList<QScrollArea*> stackContents = ui->stackedWidget->findChildren<QScrollArea*>();

    // for each scrollarea widget
    for (QScrollArea* scroll : stackContents){
        // Create a list of all QCheckBoxes in the QScrollArea
        QList<QCheckBox *> allBoxes = scroll->findChildren<QCheckBox*>();
        // Iterate through List of checkboxes
        for(QCheckBox *checkbox : allBoxes) {
            if(checkbox->isChecked()) {
                Dname = checkbox->text().section('-',0,0);
                Cnum = checkbox->text().section('-',1,1);
                Cname = checkbox->text().section('-',2,2);
                squery = "SELECT CC_ID, units FROM Course WHERE Major='"+get_major_code()+"' && DName='"+Dname+"' && CNum='"+Cnum+"';";
                query.exec(squery);

                if (query.first()) {
                    cc_id = query.value(0).toString();
                    units = query.value(1).toInt();
                } else { qDebug() << "query not found";}
                query.finish();

                squery = "SELECT units_required FROM Course_Core WHERE CC_ID = '"+cc_id+"';";
                query.exec(squery);
                if (query.first()) {
                    units_req = query.value(0).toInt();
                } else { qDebug() << "query not found";}
                query.finish();

                if (_core_sections.contains(cc_id)) {
                    int section_units = _core_sections.value(cc_id).at(0);
                    int taken_units = section_units + units;
                    _core_sections[cc_id].insert(0, taken_units);
                    _core_sections[cc_id].insert(1, units_req);
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

    set_not_in("");

    // Iterates through the QHash object and appends sections that the user have completed
    for (QHash<QString, QVector<int>>::iterator it = _core_sections.begin();it != _core_sections.end();it++) {
        if(it.value().at(0) >= it.value().at(1)) {
            _not_in.append("'"+it.key()+"',");
        }
    }

    if (!_not_in.isEmpty()) {
        _not_in.back() = ' ';
        squery = "SELECT * FROM Course WHERE Major='"+get_major_code()+"' && CatYear='"+get_catalog_year()+"' && CC_ID NOT IN ("+_not_in+");";
    }
    else {
        squery = "SELECT * FROM Course WHERE Major='"+get_major_code()+"' && CatYear='"+get_catalog_year()+"';";

    }
    query.exec(squery);

    // Creates a new layout/frame to show the user courses that they should take
    if (query.isActive()) {
        QScrollArea* newScrollArea = new QScrollArea(ui->scrollAreaWidgetContentsResults);
        QFrame* frame = new QFrame(newScrollArea);
        QVBoxLayout* vBoxLayout = new QVBoxLayout(frame);

        while(query.next()) {
            QString Dname = query.value(5).toString();
            QString Cnum = query.value(0).toString();
            QString Cname = query.value(2).toString();
            QCheckBox *checkbox = new QCheckBox(Dname + "-" + Cnum + "-" + Cname, this);
            vBoxLayout->addWidget(checkbox);
        }

        // If there is layout in the scrollarea, delete and set the new layout
        if (ui->scrollAreaWidgetContentsResults->layout()) {
            delete ui->scrollAreaWidgetContentsResults->layout();
            ui->scrollAreaWidgetContentsResults->setLayout(vBoxLayout);
        }
        else {
            ui->scrollAreaWidgetContentsResults->setLayout((vBoxLayout));
        }
    }

    query.finish();

    // clears the section vector
    if(!_core_sections.isEmpty()) {
        _core_sections.clear();
    }
}

// Controls what scrollarea is shown depending on the current section selected
void MainWindow::on_comboBoxSection_currentTextChanged(const QString &arg1)
{

    if (_stack_widget_sections.contains(arg1)) {
        ui->stackedWidget->setCurrentIndex(_stack_widget_sections.value(arg1));
    }
    else {
        qDebug() << "cannot find section";
    }
}

// If the advisor button is clicked, open advisor information window
void MainWindow::on_advisorButton_clicked()
{
    contact_ui = new Advisors_info(this);
    contact_ui->show_advisor(_major_code);
    contact_ui->show();
}

