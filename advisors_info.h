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
