#include "deditgroup.h"
#include "ui_d_editgroup.h"

//=========================================================
DEditGroup::DEditGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DEditGroup)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DEditGroup::~DEditGroup()
{
    delete ui;
}

//=========================================================
void DEditGroup::setGroupName(QString name)
{
    ui->lineEditName->setText(name);
}

//=========================================================
QString DEditGroup::groupName()
{
    return ui->lineEditName->text();
}

//=========================================================
QString DEditGroup::groupCategory()
{
    return ui->comboBox->currentText();
}

//=========================================================
void DEditGroup::slotApply()
{
    accept();
}

//=========================================================
void DEditGroup::slotCancel()
{
    reject();
}
