#include "dedituser.h"
#include "ui_d_edituser.h"

//=========================================================
DEditUser::DEditUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DEditUser)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DEditUser::~DEditUser()
{
    delete ui;
}

//=========================================================
void DEditUser::setLastName(QString name)
{
    ui->lineEditLastName->setText(name);
}

//=========================================================
void DEditUser::setFirstName(QString name)
{
    ui->lineEditFirstName->setText(name);
}

//=========================================================
void DEditUser::setMiddleName(QString name)
{
    ui->lineEditMiddleName->setText(name);
}

//=========================================================
void DEditUser::setPosition(QString position)
{
    ui->lineEditPosition->setText(position);
}

//=========================================================
int DEditUser::idGroup()
{
    int id = 0;
    for(auto& item: m_groups)
    {
        if(item.second == ui->comboBoxGroup->currentText())
            id = item.first;
    }
    return id;
}

//=========================================================
const QString DEditUser::lastName() const
{
    return ui->lineEditLastName->text();
}

//=========================================================
const QString DEditUser::firstName() const
{
    return ui->lineEditFirstName->text();
}

//=========================================================
const QString DEditUser::middleName() const
{
    return ui->lineEditMiddleName->text();
}

//=========================================================
const QString DEditUser::position() const
{
    return ui->lineEditPosition->text();
}

//=========================================================
void DEditUser::addGroup(std::map<int,QString> groups, QString currentGroup)
{
    m_groups = groups;
    ui->comboBoxGroup->clear();
    for(auto& item: groups)
    {
        ui->comboBoxGroup->addItem(item.second);
    }
    for(int i=0; i<ui->comboBoxGroup->count(); ++i)
    {
        if(ui->comboBoxGroup->itemText(i) == currentGroup)
        {
            ui->comboBoxGroup->setCurrentIndex(i);
        }
    }
}

//=========================================================
void DEditUser::slotApply()
{
    accept();
}

//=========================================================
void DEditUser::slotCancel()
{
    reject();
}

