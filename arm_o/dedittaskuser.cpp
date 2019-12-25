#include "dedittaskuser.h"
#include "ui_d_edittaskusers.h"

#include <QMessageBox>

//=========================================================
DEditTaskUser::DEditTaskUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_EditTaskUser)
{
    ui->setupUi(this);
}

//=========================================================
void DEditTaskUser::slotApply()
{
    accept();
}

//=========================================================
void DEditTaskUser::slotCancel()
{
}

//=========================================================
void DEditTaskUser::slotExit()
{
    reject();
}

