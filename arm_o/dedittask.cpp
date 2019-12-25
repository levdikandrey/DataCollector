#include "dedittask.h"
#include "ui_d_edittask.h"

#include <QMessageBox>

//=========================================================
DEditTask::DEditTask(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_EditTask)
{
    ui->setupUi(this);
}

//=========================================================
void DEditTask::slotApply()
{
    accept();
}

//=========================================================
void DEditTask::slotCancel()
{
}

//=========================================================
void DEditTask::slotExit()
{
    reject();
}

