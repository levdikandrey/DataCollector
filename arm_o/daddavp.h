#ifndef DADDAVP_H
#define DADDAVP_H

#include <QDialog>
#include <QSqlQuery>
#include "cimportdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class D_AddAVP; }
QT_END_NAMESPACE

class DAddAVP: public QDialog
{
    Q_OBJECT
public:
    DAddAVP(QWidget *parent = nullptr);
    ~DAddAVP();

private slots:
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    Ui::D_AddAVP *ui;
    CImportData *m_cImportData;
    QSqlQuery *query;

    bool addAVP();
};

#endif // DADDAVP_H
