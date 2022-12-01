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

QString Advisors_info::get_department() { return _department;}
QString Advisors_info::get_email() {return _email;}
QString Advisors_info::get_name() {return _name;}
QString Advisors_info::get_phone() {return _phone;}

void Advisors_info::show_advisor(QString major_code) {
    QSqlQuery query;
    QString squery = "SELECT * FROM Advisors WHERE DName='"+major_code+"';";

    query.exec(squery);

    if (query.isActive()) {
        while (query.next()) {
            set_department(query.value(3).toString());
            set_email(query.value(1).toString());
            set_name(query.value(0).toString());
            set_phone(query.value(2).toString());
        }
    }
    advisor_ui->label->setText(get_name());
    advisor_ui->label_2->setText(get_department());
    advisor_ui->label_3->setText(get_email());
    advisor_ui->label_4->setText(get_phone());
}
