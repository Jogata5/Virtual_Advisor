#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlineedit.h"
#include <QMainWindow>

#include <QComboBox>
#include <QPushButton>


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

private:
    Ui::MainWindow *ui;
    QString major = "";
    QString catalog_year = "";
    QString cwid;
    QString name;
};

#endif // MAINWINDOW_H
