#ifndef DGROUP_H
#define DGROUP_H

#include <QDialog>
#include <QSqlQuery>
#include "d_addgroup.h"
#include "deditgroup.h"

QT_BEGIN_NAMESPACE
namespace Ui { class D_Group; }
QT_END_NAMESPACE

class DGroup: public QDialog
{
    Q_OBJECT
public:
    explicit DGroup(QWidget *parent = nullptr);
    ~DGroup();
    void initTableGroup();

private slots:
    void slotAdd();
    void slotDelete();
    void slotEdit();
    void slotEdit(int, int);
    void slotExit();

private:
    QSqlQuery *query;
    D_AddGroup *m_dAddGroup;
    DEditGroup *m_dEditGroup;
    Ui::D_Group *ui;
};

#endif // DGROUP_H
