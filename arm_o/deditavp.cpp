#include "deditavp.h"
#include "ui_d_editavp.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

extern QSqlDatabase db;
//=========================================================
DEditAVP::DEditAVP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DEditAVP)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
}

//=========================================================
DEditAVP::~DEditAVP()
{
    delete ui;
}

//=========================================================
void DEditAVP::initAVS(uint64_t idAVP)
{
    QString sql="",tmp, str;
    m_idAVP = idAVP;
    try
    {
        sql = "SELECT avp.\"NameRus\",avp.\"NameOriginal\",avp.\"URL\",aa.\"Form\",aa.\"Rubric\",aa.\"FilmMaker\",aa.\"Age\",aa.\"YearOfRelease\",aa.\"Duration\" FROM avp "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\"=avp.\"ID\" WHERE avp.\"ID\"="+tmp.setNum(idAVP)+";";
        qDebug()<<"SQL="<<sql;
        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->lineEditName->setText(query->value(0).toString());
                ui->lineEditNameOriginal->setText(query->value(1).toString());
                ui->lineEditURL_AVP->setText(query->value(2).toString());
                ui->lineEditRubric->setText(query->value(4).toString());
                ui->lineEditFilmMaker->setText(query->value(5).toString());
                ui->lineEditAge->setText(query->value(6).toString());
                ui->lineEditYearOfRelease->setText(query->value(7).toString());
                ui->lineEditDuration->setText(query->value(8).toString());
            }
        }
        else
        {
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
const QString DEditAVP::getNameAVP() const
{
    return ui->lineEditName->text();
}

//=========================================================
const QString DEditAVP::getNameOriginal() const
{
    return ui->lineEditNameOriginal->text();
}

//=========================================================
const QString DEditAVP::getURL_AVP() const
{
    return ui->lineEditURL_AVP->text();
}

//=========================================================
const QString DEditAVP::getRubricAVP() const
{
    return ui->lineEditRubric->text();
}

//=========================================================
const QString DEditAVP::getFilmMaker() const
{
    return ui->lineEditFilmMaker->text();
}

//=========================================================
const QString DEditAVP::getAge() const
{
    return ui->lineEditAge->text();
}

//=========================================================
const QString DEditAVP::getYearOfRelease() const
{
    return ui->lineEditYearOfRelease->text();
}

//=========================================================
const QString DEditAVP::getDuration() const
{
    return ui->lineEditDuration->text();
}

//=========================================================
void DEditAVP::slotApply()
{
    if(QMessageBox::warning(this,"Внимание","Вы действительно хотите сохранить введенные изменения?","Да","Нет")==0)
        accept();
}

//=========================================================
void DEditAVP::slotCancel()
{
    ui->lineEditName->clear();
    ui->lineEditNameOriginal->clear();
    ui->lineEditURL_AVP->clear();
    ui->lineEditRubric->clear();
    ui->lineEditFilmMaker->clear();
    ui->lineEditAge->clear();
    ui->lineEditYearOfRelease->clear();
    ui->lineEditDuration->clear();
    initAVS(m_idAVP);
}

//=========================================================
void DEditAVP::slotExit()
{
    reject();
}
