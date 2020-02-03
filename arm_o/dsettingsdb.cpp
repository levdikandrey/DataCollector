#include "dsettingsdb.h"
#include "ui_d_settingsdb.h"

//=========================================================
DSettingsDB::DSettingsDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSettingsDB)
{
    ui->setupUi(this);
}

//=========================================================
DSettingsDB::~DSettingsDB()
{
    delete ui;
}

//=========================================================
void DSettingsDB::slotApply()
{
    accept();
}

//=========================================================
void DSettingsDB::slotCancel()
{
    reject();
}
