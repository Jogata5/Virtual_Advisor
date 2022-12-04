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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// MainWindow
#include <QMainWindow>

// SQL
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>

// Widgets and Objects
#include <QComboBox>
#include <QHash>
#include <QMessageBox>
#include <QPair>
#include <QPushButton>
#include <QStackedWidget>
#include <QVector>

#include "advisors_info.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Prototype  method for connecting to local database
    void connect_database(QString database_type, QString host, QString name, QString user, QString password, int port);

    // Prototypes for Setters
    void set_catalog_year(QString cat_year);
    void set_cwid(QString cwid);
    void set_major(QString major);
    void set_major_code(QString code);
    void set_name(QString name);
    void set_not_in(QString not_in);

    // Prototypes for Getters
    QSqlDatabase get_database();
    QString get_catalog_year();
    QString get_cwid();
    QString get_major();
    QString get_major_code();
    QString get_name();

    // slots/methods that are connected to each interactable UI object
private slots:
    void on_comboBoxMajors_activated(int index);

    void on_submitButton_clicked();

    void on_errorSubmit_clicked();

    void on_submitCoursesButton_clicked();

    void on_comboBoxSection_currentTextChanged(const QString &arg1);

    void on_advisorButton_clicked();

private:
    Ui::MainWindow *ui;
    Advisors_info *contact_ui;

    QString _catalog_year = "";
    QString _cwid = "";
    QString _major = "";
    QString _major_code = "";
    QString _name = "";
    QString _not_in = "";

    QSqlDatabase _mDatabase;

    QHash<QString, QVector<int>> _core_sections;
    QHash<QString, int> _stack_widget_sections;

    QVector<QPair<QString, QString>> _major_pairs;
    QVector<QString> _sections;
};

#endif // MAINWINDOW_H
