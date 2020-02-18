#ifndef DCHANGEPASSWORD_H
#define DCHANGEPASSWORD_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DChangePassword;
}

class DChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit DChangePassword(QWidget *parent = nullptr);
    ~DChangePassword();

public slots:
    void slotApply();
    void slotCancel();
    void clear();

private:
    bool checkPassword(QString fio);
    Ui::DChangePassword *ui;
    QSqlQuery *query;
};

#endif // DCHANGEPASSWORD_H
