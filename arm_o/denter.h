#ifndef D_ENTER_H
#define D_ENTER_H

#include <QDialog>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class D_Enter; }
QT_END_NAMESPACE

class DEnter: public QDialog
{
    Q_OBJECT
public:
    DEnter(QWidget *parent = nullptr);
    QString currentUserName1();
    void initUserComBoBox();

private slots:
    void slotEnter();
    void slotCancel();

private:
    bool checkPassword(QString fio);
    Ui::D_Enter *ui;
    QSqlQuery *query;
};

#endif // D_ENTER_H
