#include "dsettingsdb.h"
#include "ui_d_settingsdb.h"

//=========================================================
DSettingsDB::DSettingsDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSettingsDB)
{
    ui->setupUi(this);
    initDialog();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DSettingsDB::~DSettingsDB()
{
    delete ui;
}

//=========================================================
void DSettingsDB::initDialog()
{
//    qDebug() << QDir::toNativeSeparators(QApplication::applicationDirPath()) + "/settings.ini";
    settings = new QSettings(QDir::toNativeSeparators(QApplication::applicationDirPath()) + "/settings.ini",QSettings::IniFormat);
    ui->lineEditIP->setText(settings->value("MAIN/SERVER").toString());
    ui->lineEditPort->setText(settings->value("MAIN/PORT").toString());
    ui->lineEditNameDB->setText(settings->value("MAIN/NAME_DB").toString());
    ui->lineEditUser->setText(settings->value("MAIN/USER").toString());
    ui->lineEditPassword->setText(settings->value("MAIN/PASSWD").toString());
}

//=========================================================
void DSettingsDB::slotApply()
{
    settings->setValue("MAIN/SERVER",ui->lineEditIP->text());
    settings->setValue("MAIN/PORT",ui->lineEditPort->text());
    settings->setValue("MAIN/NAME_DB",ui->lineEditNameDB->text());
    settings->setValue("MAIN/USER",ui->lineEditUser->text());
    settings->setValue("MAIN/PASSWD",ui->lineEditPassword->text());
    accept();
}

//=========================================================
void DSettingsDB::slotCancel()
{
    reject();
}
