#include "dedittaskuser.h"
#include "ui_d_edittaskusers.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>
#include <QFileDialog>
#include <QProcess>

extern QSqlDatabase db;
//=========================================================
DEditTaskUser::DEditTaskUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_EditTaskUser)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(0, 150);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(2, 90);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(3, 0);

    flagEditOperator = false;
    flagEditStatus = false;
    flagEditPriority = false;
    flagEditPercent = false;
    flagEditComment = false;
    flagEditDate = false;

    connect(ui->comboBoxUser, QOverload<int>::of(&QComboBox::activated),[=](int){ flagEditOperator = true; });
    connect(ui->comboBoxStatus, QOverload<int>::of(&QComboBox::activated),[=](int){ flagEditStatus = true; });
    connect(ui->comboBoxPriority, QOverload<int>::of(&QComboBox::activated),[=](int){ flagEditPriority = true; });
    connect(ui->spinBoxPercent, QOverload<int>::of(&QSpinBox::valueChanged),[=](int){flagEditPercent = true; });
    connect(ui->dateEdit, SIGNAL(dateChanged(const QDate &)), this, SLOT(slotDateChanged(const  QDate &)));
}

//=========================================================
DEditTaskUser::~DEditTaskUser()
{
    delete query;
    delete ui;
}

//=========================================================
void DEditTaskUser::slotDateChanged(const  QDate &)
{
    flagEditDate = true;
}

//=========================================================
void DEditTaskUser::initComboBoxUser(QString currentUser)
{
    QString sql="";
    m_user = currentUser;
    try
    {
        ui->comboBoxUser->clear();

        sql = "SELECT \"ID\",\"FIO\" FROM \"User\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxUser->addItem(query->value(1).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->comboBoxUser->setCurrentText(currentUser);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
void DEditTaskUser::initComboBoxPriority(QString currentPriority)
{
    QString sql="";
    m_priority = currentPriority;
    try
    {
        ui->comboBoxPriority->clear();

        sql = "SELECT \"NamePriority\" FROM \"Priority\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxPriority->addItem(query->value(0).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->comboBoxPriority->setCurrentText(currentPriority);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTaskUser::initComboBoxStatus(QString currentStatus)
{
    QString sql="";
    m_status = currentStatus;
    try
    {
        ui->comboBoxStatus->clear();

        sql = "SELECT \"StatusName\",\"ID\" FROM \"TaskStatus\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                if((query->value(1).toInt() != 5) && (query->value(1).toInt() != 6))
                    ui->comboBoxStatus->addItem(query->value(0).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->comboBoxStatus->setCurrentText(currentStatus);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTaskUser::initDateRealization(long id_avp)
{
    QString sql, tmp;
    m_idAVP = id_avp;
    try
    {
        sql = "SELECT \"DateRealization\" FROM \"Task\" WHERE \"ID_AVP\"="+tmp.setNum(id_avp)+";";
        if(query->exec(sql))
        {
            if(query->next())
            {
                if(!query->value(0).isNull())
                    ui->dateEdit->setDate(query->value(0).toDate());
                else
                    ui->dateEdit->setDate(QDate::currentDate());
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
void DEditTaskUser::initTableViolation(long id_avp)
{
    QString sql="",tmp, str;

    ui->tableWidgetViolation->clearContents();
    ui->tableWidgetViolation->setRowCount(0);
    m_idAVP = id_avp;

    try
    {
        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\",ar.\"ID\",c.\"Data\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" LEFT JOIN \"Content\" c ON ar.\"ID\" = c.\"ID_AR\" WHERE ar.\"ID_AVP\"="+tmp.setNum(id_avp)+";";
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
//                if(query->value(0).toString()!="Не обнаружено")
//                {
                    ui->tableWidgetViolation->setRowCount(row+1);

                    QTableWidgetItem *newItem = new QTableWidgetItem();
                    newItem->setText(query->value(0).toString());
                    newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,0, newItem);//Название нарушения

                    QTableWidgetItem *newItem1 = new QTableWidgetItem();
                    newItem1->setText(query->value(2).toString());
                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,1, newItem1);//Описание нарушения

                    QIcon icon1;
                    QPushButton *pbItem = new QPushButton(this);
                    connect(pbItem, SIGNAL(clicked()),this,SLOT(slotViewPicture()));
                    if(query->value(4).isNull())
                    {
                        icon1.addFile(QString::fromUtf8(":/icons/icons/unspin.png"), QSize(), QIcon::Normal, QIcon::Off);
                        pbItem->setText("Нет");
                    }
                    else
                        icon1.addFile(QString::fromUtf8(":/icons/icons/attach.png"), QSize(), QIcon::Normal, QIcon::Off);

                    pbItem->setIcon(icon1);
                    ui->tableWidgetViolation->setCellWidget(row,2, pbItem);//Снимок экрана

                    QTableWidgetItem *newItem3 = new QTableWidgetItem();
                    newItem3->setText(query->value(3).toString());
                    newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,3, newItem3);//ID
                    row++;
//                }
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
void DEditTaskUser::slotViewPicture()
{
    QByteArray data;
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    QString sql;
    sql = "SELECT \"Data\" FROM \"Content\" WHERE \"ID_AR\" = ";
    sql += ui->tableWidgetViolation->item(selectedRows[0].row(),3)->text(); sql += ";";

    if(query->exec(sql))
    {
        if(query->next())
            data = query->value(0).toByteArray();

        QFile file("./tmp.jpg");
        if(file.open(QFile::WriteOnly))
        {
            file.write(data);
            qDebug()<<"data.size()="<<data.size();
            file.close();
        }
        else
            qDebug()<<"Не могу открыть файл: ./tmp.jpg";
//        QDesktopServices::openUrl(QUrl("./tmp.jpg"));
        if(data.size() == 0)
        {
            QMessageBox::warning(this, tr("Внимание"),tr("Нет прикрепленного изображения!"),tr("Да"));
        }
        else
        QProcess::startDetached("mspaint ./tmp.jpg");
    }
    else
    {
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
void DEditTaskUser::setNameAVP(QString nameAVP)
{
    ui->labelNameAVP->setText(nameAVP);
}

//=========================================================
void DEditTaskUser::setPercent(QString percent)
{
    QString value;
    value = percent.mid(0,percent.length()-1);
    m_percent = percent;
    ui->spinBoxPercent->setValue(value.toInt());
}

//=========================================================
void DEditTaskUser::setComment(QString comment)
{
    m_comment = comment;
    ui->textEditComment->setText(comment);
}

//=========================================================
const QString DEditTaskUser::getNameUser() const
{
    return ui->comboBoxUser->currentText();
}

//=========================================================
const QString DEditTaskUser::getPriority() const
{
    return ui->comboBoxPriority->currentText();
}

//=========================================================
const QString DEditTaskUser::getStatus() const
{
    return ui->comboBoxStatus->currentText();
}

//=========================================================
const QString DEditTaskUser::getPercent() const
{
    QString tmp;
    return tmp.setNum(ui->spinBoxPercent->value());
}

//=========================================================
const QString DEditTaskUser::getComment() const
{
    return ui->textEditComment->toPlainText();
}

//=========================================================
const QString DEditTaskUser::getDateRealization() const
{
    return ui->dateEdit->dateTime().toString("yyyy-MM-ddTHH:mm:ss");
}

//=========================================================
QTableWidget* DEditTaskUser::getViolations() const
{
    return ui->tableWidgetViolation;
}

//=========================================================
void DEditTaskUser::slotApply()
{
    accept();
}

//=========================================================
void DEditTaskUser::slotCancel()
{
    ui->comboBoxUser->setCurrentText(m_user);
    ui->comboBoxPriority->setCurrentText(m_priority);
    ui->comboBoxStatus->setCurrentText(m_status);
    ui->spinBoxPercent->setValue(m_percent.toInt());
    ui->textEditComment->setText(m_comment);
}

//=========================================================
void DEditTaskUser::slotExit()
{
    reject();
}

