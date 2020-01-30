#include "daddavp.h"
#include "ui_d_addavp.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSpinBox>

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
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DAddAVP::~DAddAVP()
{
    if(query!=nullptr)
        delete query;
    delete m_cImportData;
}

//=========================================================
void DAddAVP::initAVS()
{
    QString sql="";
    QString itemAVS;
    try
    {
       ui->comboBoxNameAVS->clear();
       listAVS.clear();
       sql="SELECT \"ID\",\"URL\",\"NameAVS\" FROM avs;";
       if(query->exec(sql))
       {
           int i=0;
           while(query->next())
           {
               itemAVS = query->value(2).toString();itemAVS +=" (";
               itemAVS += query->value(1).toString();itemAVS +=" )";
               ui->comboBoxNameAVS->addItem(itemAVS);
               if(i==0)
                    ui->lineEditURL_AVS->setText(query->value(1).toString());
               listAVS.insert(std::make_pair(i,query->value(1).toString()));
               i++;
           }
       }
       else
           qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
const QString DAddAVP::getNameAVS() const
{
    return ui->comboBoxNameAVS->currentText();
}

//=========================================================
const QString DAddAVP::getNameAVP() const
{
    return ui->lineEditName->text();
}

//=========================================================
const QString DAddAVP::getNameOriginal() const
{
    return ui->lineEditNameOriginal->text();
}

//=========================================================
const QString DAddAVP::getURL_AVS() const
{
    return ui->lineEditURL_AVS->text();
}

//=========================================================
const QString DAddAVP::getURL_AVP() const
{
    return ui->lineEditURL_AVP->text();
}

//=========================================================
const QString DAddAVP::getFormAVP() const
{
    return ui->lineEditFormAVP->text();
}

//=========================================================
const QString DAddAVP::getRubricAVP() const
{
    return ui->lineEditRubric->text();
}

//=========================================================
const QString DAddAVP::getFilmMaker() const
{
    return ui->lineEditFilmMaker->text();
}

//=========================================================
const QString DAddAVP::getAge() const
{
    return ui->lineEditAge->text();
}

//=========================================================
const QString DAddAVP::getYearOfRelease() const
{
    QString tmp;
    return tmp.setNum(ui->spinBoxYearOfRelease->value());
}

//=========================================================
const QString DAddAVP::getDuration() const
{
    QString tmp;
    return tmp.setNum(ui->spinBoxDuration->value());
}

//=========================================================
void DAddAVP::slotActivated(int item)
{
    QString sql="";
    QString itemAVS;
    ui->lineEditURL_AVS->setText(listAVS.find(item)->second);
}

//=========================================================
void DAddAVP::slotApply()
{
    if( (ui->lineEditName->text()=="") ||
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
    ui->spinBoxDuration->setValue(2000);
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
        m_cImportData->m_sDataAVP.avsName = ui->comboBoxNameAVS->currentText();
        m_cImportData->m_sDataAVP.avpNameRus = ui->lineEditName->text();
        m_cImportData->m_sDataAVP.avpNameOriginal = ui->lineEditNameOriginal->text();
        m_cImportData->m_sDataAVP.avsURL = ui->lineEditURL_AVS->text();
        m_cImportData->m_sDataAVP.avpURL = ui->lineEditURL_AVP->text();
        m_cImportData->m_sDataAVP.avpForm = ui->lineEditFormAVP->text();
        m_cImportData->m_sDataAVP.rubric = ui->lineEditRubric->text();
        m_cImportData->m_sDataAVP.filmMaker = ui->lineEditFilmMaker->text();
        m_cImportData->m_sDataAVP.age = ui->lineEditAge->text();
        m_cImportData->m_sDataAVP.yearOfRelease = tmp.setNum(ui->spinBoxYearOfRelease->value());
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
