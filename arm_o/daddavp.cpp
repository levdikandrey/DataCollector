#include "daddavp.h"
#include "ui_d_addavp.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>

extern QSqlDatabase db;
extern QString currentUserName;
//=========================================================
DAddAVP::DAddAVP(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_AddAVP)
{
    ui->setupUi(this);
    query = nullptr;
    query = new QSqlQuery(db);
    m_cImportData = new CImportData();
}

//=========================================================
DAddAVP::~DAddAVP()
{
    if(query!=nullptr)
        delete query;
    delete m_cImportData;
}

//=========================================================
void DAddAVP::slotApply()
{
    if( (ui->lineEditName->text()=="") ||
            (ui->lineEditNameAVS->text()=="") ||
            (ui->lineEditURL_AVS->text()=="") ||
            (ui->lineEditURL_AVP->text()=="") ||
            (ui->lineEditFormAVP->text()=="") ||
            (ui->lineEditRubric->text()=="")  ||
            (ui->lineEditFilmMaker->text()==""))
    {
        QMessageBox::warning(this, tr("Внимание"),tr("Не все обязательные поля отмеченные (*) заполнены!"),tr("Да"));
        return;
    }

    if(addAVP())
        accept();
    else
    {}
}

//=========================================================
void DAddAVP::slotCancel()
{
    ui->lineEditName->setText("");
    ui->lineEditNameOriginal->setText("");
    ui->lineEditURL_AVS->setText("");
    ui->lineEditURL_AVP->setText("");
    ui->lineEditFormAVP->setText("");
    ui->lineEditRubric->setText("");
    ui->lineEditFilmMaker->setText("");
    ui->lineEditAge->setText("");
    ui->dateEditYearOfRelease->setDate(QDate(2000,1,1));
    ui->spinBoxDuration->setValue(0);
}

//=========================================================
bool DAddAVP::addAVP()
{
    bool res =true;
    QString sql="";
    QString tmp;
    try
    {
        m_cImportData->m_sDataAVP.avsName = ui->lineEditNameAVS->text();
        m_cImportData->m_sDataAVP.avpNameRus = ui->lineEditName->text();
        m_cImportData->m_sDataAVP.avpNameOriginal = ui->lineEditNameOriginal->text();
        m_cImportData->m_sDataAVP.avsURL = ui->lineEditURL_AVS->text();
        m_cImportData->m_sDataAVP.avpURL = ui->lineEditURL_AVP->text();
        m_cImportData->m_sDataAVP.avpForm = ui->lineEditFormAVP->text();
        m_cImportData->m_sDataAVP.rubric = ui->lineEditRubric->text();
        m_cImportData->m_sDataAVP.filmMaker = ui->lineEditFilmMaker->text();
        m_cImportData->m_sDataAVP.age = ui->lineEditAge->text();
        m_cImportData->m_sDataAVP.yearOfRelease = ui->dateEditYearOfRelease->date().toString("dd.MM.yyyy");
        m_cImportData->m_sDataAVP.duration = tmp.setNum(ui->spinBoxDuration->value());
        m_cImportData->m_sDataAVP.dateSaveInDB = QDateTime::currentDateTime();
        m_cImportData->m_sDataAVP.userSaveInDB = currentUserName;
        m_cImportData->addSaveInDB(m_cImportData->m_sDataAVP);

    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
    }
    return res;
}

//=========================================================
void DAddAVP::slotExit()
{
    reject();
}
