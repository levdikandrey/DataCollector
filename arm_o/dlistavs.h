#ifndef DLISTAVS_H
#define DLISTAVS_H

#include <QDialog>
#include <QSqlQuery>
#include "daddavs.h"

namespace Ui {
class DListAVS;
}

class DListAVS : public QDialog
{
    Q_OBJECT

public:
    explicit DListAVS(QWidget *parent = nullptr);
    ~DListAVS();
    void initDialog();

private slots:
    void slotAddAVS();
    void slotDeleteAVS();
    void slotEditAVS();

private:
    Ui::DListAVS *ui;
    QSqlQuery *query;
    DAddAVS *dAddAVS;
};

#endif // DLISTAVS_H
