#include "d_addgroup.h"
#include "ui_d_addgroup.h"

#include <QMessageBox>

//=========================================================
D_AddGroup::D_AddGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::D_AddGroup)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
D_AddGroup::~D_AddGroup()
{
    delete ui;
}

//=========================================================
QString D_AddGroup::groupName()
{
    return ui->lineEditName->text();
}

//=========================================================
QString D_AddGroup::groupCategory()
{
    return ui->comboBox->currentText();
}

//=========================================================
void D_AddGroup::slotApply()
{
    if(ui->lineEditName->text()=="")
    {
        QMessageBox::warning(this, tr("Внимание"),tr("Введите название группы!"),tr("Да"));
        return;
    }
    accept();
}

//=========================================================
void D_AddGroup::slotCancel()
{
    reject();
}
