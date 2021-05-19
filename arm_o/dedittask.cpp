#include "dedittask.h"
#include "ui_d_edittask.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>
#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>
#include <QTimeEdit>

extern QSqlDatabase db;
//=========================================================
DEditTask::DEditTask(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_EditTask)
{
    ui->setupUi(this);
    ui->groupBox_2->hide();
    ui->groupBoxComment->hide();

    query = new QSqlQuery(db);
    query_data = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(0, 150);
//    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(2, 90);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(3, 0);
    flagConnected = false;

    connect(ui->radioButton_4,SIGNAL(clicked(bool)), SLOT(slotFree(bool)));
    connect(ui->radioButton,SIGNAL(clicked(bool)), SLOT(slotPaid(bool)));
    connect(ui->radioButton_2,SIGNAL(clicked(bool)), SLOT(slotPurchased(bool)));
    connect(ui->radioButton_3,SIGNAL(clicked(bool)), SLOT(slotRent(bool)));
    ui->label_3->hide();
    ui->spinBoxPercent->hide();
    dInfoAVP = new DInfoAVP(this);
}

//=========================================================
void DEditTask::slotFree(bool f)
{
    if(!f)
        return;
    qDebug()<<__PRETTY_FUNCTION__;
    ui->spinBox->setEnabled(false);
    QString sql="",tmp;
    try
    {
        sql = "UPDATE avp SET \"Attribute\" = 0 ";
        sql+="WHERE \"ID\"=";
        sql+=tmp.setNum(m_idAVP); sql+=";";
        qDebug()<<"sql = "<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
        ((MainWindow*)parent())->initTableMyTask();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTask::slotPaid(bool f)
{
    if(!f)
        return;
    qDebug()<<__PRETTY_FUNCTION__;
    ui->spinBox->setEnabled(false);
    QString sql="",tmp;
    try
    {
        sql = "UPDATE avp SET \"Attribute\" = 1 ";
        sql+="WHERE \"ID\"=";
        sql+=tmp.setNum(m_idAVP); sql+=";";
        qDebug()<<"sql = "<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
        ((MainWindow*)parent())->initTableMyTask();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTask::slotPurchased(bool f)
{
    if(!f)
        return;
    qDebug()<<__PRETTY_FUNCTION__;
    ui->spinBox->setEnabled(false);
    QString sql="",tmp;
    try
    {
        sql = "UPDATE avp SET \"Attribute\" = 2 ";
        sql+="WHERE \"ID\"=";
        sql+=tmp.setNum(m_idAVP); sql+=";";
        qDebug()<<"sql = "<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
        ((MainWindow*)parent())->initTableMyTask();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTask::slotRent(bool f)
{
    if(!f)
        return;
    qDebug()<<__PRETTY_FUNCTION__;
    ui->spinBox->setEnabled(true);
    QString sql="",tmp;
    try
    {
        sql = "UPDATE avp SET \"Attribute\" = 3, \"QuantityDay\"=";
        sql += tmp.setNum(ui->spinBox->value());
        sql += " WHERE \"ID\"=";
        sql += tmp.setNum(m_idAVP); sql+=";";
        qDebug()<<"sql = "<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
        ((MainWindow*)parent())->initTableMyTask();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTask::slotValueChangedDay(int day)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString sql="",tmp;
    try
    {
        sql = "UPDATE avp SET \"QuantityDay\"=";
        sql += tmp.setNum(day);
        sql += " WHERE \"ID\"=";
        sql += tmp.setNum(m_idAVP); sql+=";";
        qDebug()<<"sql = "<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
        ((MainWindow*)parent())->initTableMyTask();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}


//=========================================================
void DEditTask::slotCellChanged(QTableWidgetItem* item)
//void DEditTask::slotCellChanged(int row,int column)
{
    qDebug()<<__PRETTY_FUNCTION__;
    int row =item->row();
    qDebug()<<"row = "<<item->row();
//    qDebug()<<"colmn = "<<column;
    QString sql="";
    QString textViolation = item->text();
    qDebug()<<"textViolation = "<<textViolation;
    QString ID =ui->tableWidgetViolation->item(row,3)->text();
    qDebug()<<"ID = "<<ID;
    try
    {
        sql = "UPDATE \"AnalysisResult\" ar SET ar.\"TextViolation\" = \'";
        sql+=textViolation;
        sql+="\' WHERE \"ID\"=";
        sql+=ID; sql+=";";
        qDebug()<<"sql = "<<sql;
        query->exec(sql);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
DEditTask::~DEditTask()
{
    delete query;
    delete ui;
}

//=========================================================
void DEditTask::hideViolationGroup()
{
    ui->groupBox_2->hide();
}

//=========================================================
void DEditTask::showViolationGroup()
{
    ui->groupBox_2->show();
}

//=========================================================
long DEditTask::idViolation(QString nameViolation)
{
    QString sql="";
    long id = -1;
    sql = "SELECT \"ID\" FROM \"Violation\" WHERE \"Violation\" = \'";
    sql += nameViolation; sql += "\';";
    //    qDebug()<<"sql="<<sql;
    if(query->exec(sql))
    {
        while(query->next())
        {
            id = query->value(0).toInt();
        }
    }
    else
        qDebug()<<query->lastError().text();
//    qDebug()<<"id="<<id;
    return id;
}

//=========================================================
void DEditTask::initTableAttribute(long id_avp)
{
    QString sql="",tmp;
    try
    {
        ui->radioButton_4->setChecked(true);
        sql = "SELECT \"Attribute\",\"QuantityDay\" FROM avp WHERE \"ID\"=";
        sql += tmp.setNum(id_avp);
        sql += ";";
        qDebug()<<"sql="<<sql;
        if(query->exec(sql))
        {
            while(query->next())
            {
                if(query->value(0).toInt() == 0)
                {
                    ui->radioButton_4->setChecked(true);
                    ui->spinBox->setEnabled(false);
                }
                else if(query->value(0).toInt() == 1)
                {
                    ui->radioButton->setChecked(true);
                    ui->spinBox->setEnabled(false);
                }
                else if(query->value(0).toInt() == 2)
                {
                    ui->radioButton_2->setChecked(true);
                    ui->spinBox->setEnabled(false);
                }
                else if(query->value(0).toInt() == 3)
                {
                    ui->radioButton_3->setChecked(true);
                    ui->spinBox->setEnabled(true);
                    ui->spinBox->setValue(query->value(1).toInt());
                }
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
void DEditTask::initTableViolation(long id_avp)
{
    QString sql="",tmp, str;

    ui->tableWidgetViolation->clearContents();
    ui->tableWidgetViolation->setRowCount(0);
    m_idAVP = id_avp;
//    disconnect(ui->tableWidgetViolation,SIGNAL(itemChanged(QTableWidgetItem*)),SLOT(slotCellChanged(QTableWidgetItem*)));
    if(flagConnected == true)
    {
        disconnect(ui->tableWidgetViolation,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(slotCellChanged(QTableWidgetItem*)));
        flagConnected = false;
    }

    try
    {
        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\",ar.\"ID\",c.\"Data\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" LEFT JOIN \"Content\" c ON ar.\"ID\" = c.\"ID_AR\" WHERE ar.\"ID_AVP\"="+tmp.setNum(id_avp)+" AND ar.\"TextViolation\" IS NOT NULL;";
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
//                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,1, newItem1);//Описание нарушения
//                    connect(ui->tableWidgetViolation,SIGNAL(itemChanged(QTableWidgetItem*)),SLOT(slotCellChanged(QTableWidgetItem*)));


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
        if(flagConnected == false)
        {
            connect(ui->tableWidgetViolation,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(slotCellChanged(QTableWidgetItem*)));
            flagConnected = true;
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTask::initComboBoxStatus(QString currentStatus)
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
                if((query->value(1).toInt() != 6) &&
//                        (query->value(1).toInt() != 6) &&
                        (query->value(1).toInt() != 7) &&
                        (query->value(1).toInt() != 8) &&
                        (query->value(1).toInt() != 19) &&
                        (query->value(1).toInt() != 11) &&
                        (query->value(1).toInt() != 16) &&
                        (query->value(1).toInt() != 17) &&
                        (query->value(1).toInt() != 18) &&
                        (query->value(1).toInt() != 9))
                    ui->comboBoxStatus->addItem(query->value(0).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->labelCurrentStatus->setText(currentStatus);
        ui->comboBoxStatus->setCurrentText(currentStatus);
        if(currentStatus == "Одобрена экспертом")
        {
            ui->comboBoxStatus->clear();
            ui->comboBoxStatus->addItem("Одобрена экспертом");
            ui->comboBoxStatus->setCurrentText(currentStatus);

//            ui->spinBoxPercent->setValue(100);
//            ui->spinBoxPercent->setEnabled(false);

            ui->groupBox->hide();
            ui->toolButtonAdd->hide();
            ui->toolButtonDelete->hide();
            ui->pushButtonApply->hide();
            ui->pushButtonCancel->hide();
            ui->groupBoxComment->show();
        }
        else
        {
            ui->groupBox->show();
            ui->toolButtonAdd->show();
            ui->toolButtonDelete->show();
            ui->pushButtonApply->show();
            ui->pushButtonCancel->show();
            ui->groupBoxComment->hide();
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTask::initComboBoxViolation()
{
    QString sql="";
    try
    {
        ui->comboBoxViolation->clear();

        sql = "SELECT \"Violation\" FROM \"Violation\" WHERE \"ViolationType\"=2;";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxViolation->addItem(query->value(0).toString());

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
void DEditTask::setNameAVP(QString nameAVP)
{
    ui->labelNameAVP->setText(nameAVP);
}

//=========================================================
void DEditTask::setPercent(QString percent)
{
    QString value;
    value = percent.mid(0,percent.length()-1);
    m_percent = percent;
    ui->spinBoxPercent->setValue(value.toInt());
}

//=========================================================
void DEditTask::setComment(QString comment)
{
    ui->pushButtonApply->setEnabled(false);
    ui->pushButtonCancel->setEnabled(false);
    m_comment = comment;
    ui->textEditComment->setText(comment);
}

//=========================================================
const QString DEditTask::getStatus() const
{
    return ui->comboBoxStatus->currentText();
}

//=========================================================
const QString DEditTask::getPercent() const
{
    QString tmp;
    return tmp.setNum(ui->spinBoxPercent->value());
}

//=========================================================
const QString DEditTask::getComment() const
{
    return "";
//    return ui->textEditComment->toPlainText();
}

//=========================================================
QTableWidget* DEditTask::getViolations() const
{
    return ui->tableWidgetViolation;
}

//=========================================================
void DEditTask::slotAddViolation()
{
    ui->timeEditBegin->setTime(QTime::fromString("00:00:00"));
    ui->timeEditEnd->setTime(QTime::fromString("00:00:00"));
    ui->textEditViolation->clear();
    ui->lineEditScreenShot->clear();
    ui->groupBox_2->show();
}

//=========================================================
void DEditTask::slotDeleteViolation()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали нарушение для удаления!"),tr("Да"));
        return;
    }
    else
    {
        try
        {
            if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите удалить выбранные нарушения:?"),tr("Да"),tr("Нет")) == 0)
            {
                while (!selectedRows.empty())
                {
                    ((MainWindow*)parent())->addRecordJournalJobAVP(1,"Удаление нарушения - " + ui->tableWidgetViolation->takeItem(selectedRows[0].row(),0)->text()+". Для АВП",((MainWindow*)parent())->getNameRusAVP(m_idAVP),m_idAVP);
                    sql = "DELETE FROM \"AnalysisResult\" WHERE \"ID\"=";sql += ui->tableWidgetViolation->takeItem(selectedRows[0].row(),3)->text(); sql += ";";
//                    qDebug()<<"sql ="<<sql;
                    if(query->exec(sql))
                    {
                        ui->tableWidgetViolation->removeRow(selectedRows[0].row());
                        selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
                    }
                    else
                        qDebug()<<query->lastError().text();
                }
            }
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
        ((MainWindow*)parent())->initTableMyTask();
    }

}

//=========================================================
void DEditTask::slotAtachFile()
{
    QString fileName ="";
    fileName = QFileDialog::getOpenFileName(this,
                       QString::fromUtf8("Открыть файл"),
                       QDir::currentPath(),
                       "Все файлы изображений (*.png *.jpeg *.jpg *.jpe *.bmp *.ico *.gif *.tif *.tiff);;Все файлы (*.*)");

    if(fileName != "")
    {
        ui->lineEditScreenShot->setText(fileName);
    }
}

//=========================================================
void DEditTask::slotViewPicture()
{
    QByteArray data;
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    QString sql;
    sql = "SELECT \"Data\" FROM \"Content\" WHERE \"ID_AR\" = ";
    sql += ui->tableWidgetViolation->item(selectedRows[0].row(),3)->text(); sql += ";";
//    qDebug()<<"selectedRows[0]="<<selectedRows[0].row();
//    qDebug()<<"sql = "<<sql;
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
int DEditTask::getIdAnalysisResult()
{
    int id = -1;
    QString sql;
    sql = "SELECT MAX(\"ID\") FROM \"AnalysisResult\";";
    if(query->exec(sql))
    {
        if(query->next())
            id = query->value(0).toInt();
    }
    else
    {
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        qDebug()<<query->lastError().text();
    }
//    qDebug()<<"idAR="<<id;
    return id;
}

//=========================================================
void DEditTask::slotApplyViolation()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="",tmp;
    QString s_comment="";
    try
    {
        s_comment ="Таймкод: с " + ui->timeEditBegin->time().toString("HH:mm:ss") + " по "
                + ui->timeEditEnd->time().toString("HH:mm:ss") +". "
                + ui->textEditViolation->toPlainText();
        sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\", \"ID_Violation\",\"TextViolation\",\"Percent\",\"TimeBegin\",\"TimeEnd\") VALUES(";
        sql += tmp.setNum(m_idAVP); sql += ",";
        sql += tmp.setNum(idViolation(ui->comboBoxViolation->currentText())); sql += ",E\'";
        sql += ((MainWindow*)parent())->cImportData->decode(s_comment); sql += "\',\'100\',\'";
        sql += ui->timeEditBegin->time().toString("HH:mm:ss");sql += "\',\'";
        sql += ui->timeEditEnd->time().toString("HH:mm:ss");sql +="\');";
        qDebug()<<"SQL="<<sql;

        if(!query->exec(sql))
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            qDebug()<<query->lastError().text();
            return;
        }

        if(ui->lineEditScreenShot->text().length()>0)
        {
            sql = "INSERT INTO \"Content\"(\"ID_AVP\",\"ID_AR\",\"Data\") VALUES(:ID_AVP,:ID_AR,:Data);";
            query_data->prepare(sql);
            query_data->bindValue(":ID_AVP", m_idAVP);
            query_data->bindValue(":ID_AR", getIdAnalysisResult());
            query_data->bindValue(":Data", getData(ui->lineEditScreenShot->text()));
            if(!query_data->exec())
                qDebug()<<"ERROR:"<<query_data->lastError().text();
        }
        ((MainWindow*)parent())->addRecordJournalJobAVP(1,"Добавление нового нарушения - " + ui->comboBoxViolation->currentText()+". Для АВП",((MainWindow*)parent())->getNameRusAVP(m_idAVP),m_idAVP);
        initTableViolation(m_idAVP);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    hideViolationGroup();
    ui->pushButtonApplyViolation->setEnabled(false);

    ((MainWindow*)parent())->initTableMyTask();
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DEditTask::slotCancelViolation()
{
    ui->textEditViolation->clear();
    ui->lineEditScreenShot->clear();
    ui->pushButtonApplyViolation->setEnabled(false);
    hideViolationGroup();
}

//=========================================================
void DEditTask::slotStatusActivated(int)
{
    if(ui->comboBoxStatus->currentText()=="Проверена оператором")
        ui->spinBoxPercent->setValue(100);
    ui->pushButtonApply->setEnabled(true);
    ui->pushButtonCancel->setEnabled(true);
}

//=========================================================
void DEditTask::slotValueChanged(int)
{
    if(ui->comboBoxStatus->currentText()=="Проверена оператором")
        ui->spinBoxPercent->setValue(100);
    ui->pushButtonApply->setEnabled(true);
    ui->pushButtonCancel->setEnabled(true);
}

//=========================================================
void DEditTask::slotActivated(int)
{
    ui->pushButtonApplyViolation->setEnabled(true);
}

//=========================================================
void DEditTask::slotTextChanged()
{
    ui->pushButtonApplyViolation->setEnabled(true);
}

//=========================================================
void DEditTask::slotTextChanged(QString)
{
    ui->pushButtonApplyViolation->setEnabled(true);
}

//=========================================================
QByteArray DEditTask::getData(QString fileName)
{
//    qDebug()<<"fileName="<<fileName;
    QByteArray data;
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {
        data = file.readAll();
//        qDebug()<<"data.size()="<<data.size();
        file.close();
    }
    else
        qDebug()<<"Не могу открыть файл:"<<fileName;
    return data;
}

//=========================================================
void DEditTask::slotApply()
{
    ui->pushButtonApply->setEnabled(false);
    ui->pushButtonCancel->setEnabled(false);
    accept();
}

//=========================================================
void DEditTask::slotCancel()
{
    ui->comboBoxStatus->setCurrentText(m_status);
    ui->spinBoxPercent->setValue(m_percent.toInt());
    ui->pushButtonApply->setEnabled(false);
    ui->pushButtonCancel->setEnabled(false);
}

//=========================================================
void DEditTask::slotInfo()
{
    dInfoAVP->initTable(m_idAVP);
    dInfoAVP->exec();
}

//=========================================================
void DEditTask::slotExit()
{
    reject();
}

