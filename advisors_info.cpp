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

#include <QSqlQuery>
#include <QVBoxLayout>


#include "advisors_info.h"
#include "ui_advisors_info.h"

Advisors_info::Advisors_info(QWidget *parent) :
    QDialog(parent),
    advisor_ui(new Ui::Advisors_info)
{
    advisor_ui->setupUi(this);
}

Advisors_info::~Advisors_info()
{
    delete advisor_ui;
}

/// Setters
void Advisors_info::set_department(QString Dname) {
    _department = Dname;
}
void Advisors_info::set_email(QString email) {
    _email = email;
}
void Advisors_info::set_name(QString name) {
    _name = name;
}
void Advisors_info::set_phone(QString phone) {
    _phone = phone;
}

// Getters
QString Advisors_info::get_department() { return _department;}
QString Advisors_info::get_email() {return _email;}
QString Advisors_info::get_name() {return _name;}
QString Advisors_info::get_phone() {return _phone;}

// Shows Advisor Window
void Advisors_info::show_advisor(QString major_code) {

    // Initialize SQl Query and execute
    QSqlQuery query;
    QString squery = "SELECT * FROM Advisors WHERE DName='"+major_code+"';";
    query.exec(squery);

    // Checks if the query is active and iterates through the results
    if (query.isActive()) {
        while (query.next()) {
            set_department(query.value(3).toString());
            set_email(query.value(1).toString());
            set_name(query.value(0).toString());
            set_phone(query.value(2).toString());
        }
    }

    // Sets the window text boxes to the fetched SQL data.
    advisor_ui->label->setText(get_name());
    advisor_ui->label_2->setText(get_department());
    advisor_ui->label_3->setText(get_email());
    advisor_ui->label_4->setText(get_phone());
}
