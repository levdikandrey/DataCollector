#ifndef DADDTASK_H
#define DADDTASK_H

#include <QDialog>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class D_AddTask; }
QT_END_NAMESPACE

class DAddTask: public QDialog
{
        Q_OBJECT
public:
    DAddTask(QWidget *parent = nullptr);
    ~DAddTask();
    void initComboBoxUser();
    void initTableListAVP();

private slots:
    void slotAdd();
    void slotDelete();
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    Ui::D_AddTask *ui;
    QSqlQuery *query;
};

#endif // DADDTASK_H
