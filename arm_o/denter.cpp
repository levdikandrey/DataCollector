#include "denter.h"
#include "ui_d_enter.h"

//=========================================================
DEnter::DEnter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_Enter)
{
    ui->setupUi(this);
}

//=========================================================
QString DEnter::currentUserName()
{
    return ui->comboBoxUsers->currentText();
}

//=========================================================
void DEnter::slotEnter()
{
    accept();
}

//=========================================================
void DEnter::slotCancel()
{
    reject();
}
