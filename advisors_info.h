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

#ifndef ADVISORS_INFO_H
#define ADVISORS_INFO_H

#include <QDialog>

namespace Ui {
class Advisors_info;
}

class Advisors_info : public QDialog
{
    Q_OBJECT

public:
    explicit Advisors_info(QWidget *parent = nullptr);
    ~Advisors_info();

    void show_advisor(QString major_code);

    void set_department(QString Dname);
    void set_email(QString email);
    void set_name(QString name);
    void set_phone(QString phone);

    QString get_department();
    QString get_email();
    QString get_name();
    QString get_phone();

private:
    Ui::Advisors_info *advisor_ui;

    QString _department;
    QString _email;
    QString _name;
    QString _phone;
};

#endif // ADVISORS_INFO_H
