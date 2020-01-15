#include "daddviolation.h"
#include "ui_d_addviolation.h"

#include <QMessageBox>
//=========================================================
DAddViolation::DAddViolation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DAddViolation)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DAddViolation::~DAddViolation()
{
    delete ui;
}

//=========================================================
const QString DAddViolation::violation() const
{
    return ui->lineEditName->text();
}

//=========================================================
void DAddViolation::setViolation(QString violation)
{
    ui->lineEditName->setText(violation);
}

//=========================================================
void DAddViolation::setTitle(QString title)
{
    setWindowTitle(title);
}

//=========================================================
void DAddViolation::slotApply()
{
    if(ui->lineEditName->text()=="")
    {
        QMessageBox::warning(this, tr("Внимание"),tr("Введите наименование нарушения!"),tr("Да"));
        return;
    }
    accept();
}

//=========================================================
void DAddViolation::slotCancel()
{
    reject();
}