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
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_comboBoxMajors_activated(int index);


    void on_submitButton_clicked();

    void on_errorSubmit_clicked();

    void on_submitCoursesButton_clicked();

private:
    Ui::MainWindow *ui;
    QString major = "";
    QString catalog_year = "";
    QString cwid;
    QString name;
    QString major_code;
    QVector<QPair<QString, QString>> major_pairs;
    QSqlTableModel *mModel;
    QSqlDatabase mDatabase;
};

#endif // MAINWINDOW_H
