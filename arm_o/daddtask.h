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
    void initTableListAVP(int numberPage = 1);

private slots:
    void slotAdd();
    void slotDelete();
    void slotApply();
    void slotCancel();
    void slotExit();
    void slotPrevious();
    void slotNext();
    void slotChangeNumberPage();

private:
    Ui::D_AddTask *ui;
    QSqlQuery *query;

    int countAVP();
    int m_countAVP;
    int m_currentNumberPage;

};

#endif // DADDTASK_H
