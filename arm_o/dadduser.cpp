#include "dadduser.h"
#include "ui_d_adduser.h"

#include <QMessageBox>

//=========================================================
DAddUser::DAddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DAddUser)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DAddUser::~DAddUser()
{
    delete ui;
}

//=========================================================
const QString DAddUser::userName() const
{
    return ui->lineEditName->text();
}

//=========================================================
const QString DAddUser::range() const
{
    return ui->lineEditRange->text();
}

//=========================================================
void DAddUser::addGroup(std::map<int,QString> groups)
{
    m_groups = groups;
    ui->comboBoxGroup->clear();
    for(auto& item: groups)
    {
     ui->comboBoxGroup->addItem(item.second);
    }
}

//=========================================================
int DAddUser::idGroup()
{
    int id =0;
    for(auto& item: m_groups)
    {
        if(item.second == ui->comboBoxGroup->currentText())
            id = item.first;
    }
    return id;
}

//=========================================================
void DAddUser::slotApply()
{
    if(ui->lineEditName->text()=="")
    {
        QMessageBox::warning(this, tr("Внимание"),tr("Введите ФИО пользователя!"),tr("Да"));
        return;
    }
    if(ui->lineEditRange->text()=="")
    {
        QMessageBox::warning(this, tr("Внимание"),tr("Введите должность!"),tr("Да"));
        return;
    }
    accept();
}

//=========================================================
void DAddUser::slotCancel()
{
    reject();
}

