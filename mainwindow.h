#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>

#include <QMessageBox>
#include <QComboBox>
#include <QPushButton>
#include <QPair>
#include <QStackedWidget>
#include <QVector>
#include <QHash>

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

    void connect_database(QString database_type, QString host, QString name, QString user, QString password, int port);

    void set_catalog_year(QString cat_year);
    void set_cwid(QString cwid);
    void set_major(QString major);
    void set_major_code(QString code);
    void set_name(QString name);
    void set_not_in(QString not_in);

//    QHash<QString, QVector<int>> get_core_sections();
//    QHash<QString,int> get_stack_widget_sections();

    QSqlDatabase get_database();

    QString get_catalog_year();
    QString get_cwid();
    QString get_major();
    QString get_major_code();
    QString get_name();



//    QVector<QPair<QString, QString>> get_major_pairs();
//    QVector<QString> get_sections();


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
