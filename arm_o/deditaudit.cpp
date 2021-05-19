#include "deditaudit.h"
#include "ui_d_editaudit.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCheckBox>
#include <QFile>
#include <QProcess>
#include <QFileDialog>
#include <QDesktopServices>

extern QSqlDatabase db;
//=========================================================
DEditAudit::DEditAudit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DEditAudit)
{
    ui->setupUi(this);
    ui->groupBox_3->hide();

    query = new QSqlQuery(db);
    query_data = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->tableWidgetViolation->horizontalHeader()->resizeSection(0, 150);
//    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(2, 90);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(3, 100);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(4, 100);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(5, 0);
    m_flagAnswer = true;
    dInfoAVP = new DInfoAVP(this);
}

//=========================================================
DEditAudit::~DEditAudit()
{
    delete ui;
}

//=========================================================
void DEditAudit::initTableViolation(long id_avp)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<"id_avp="<<id_avp;
    QString sql="",tmp, str;
    bool flagEdit = false;

    ui->tableWidgetViolation->clearContents();
    ui->tableWidgetViolation->setRowCount(0);
    m_idAVP = id_avp;

    try
    {
        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\",ar.\"ID\",c.\"Data\",ar.\"CheckExpert\",ar.\"CheckExpertCancel\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" LEFT JOIN \"Content\" c ON ar.\"ID\" = c.\"ID_AR\" WHERE ar.\"ID_AVP\"="+tmp.setNum(id_avp)+" AND ar.\"TextViolation\" IS NOT NULL;";
        qDebug()<<"sql="<<sql;
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
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
//                    connect(pbItem, SIGNAL(clicked()),this,SLOT(slotViewPicture()));
                    ui->tableWidgetViolation->setCellWidget(row,2, pbItem);//Снимок экрана

                    QCheckBox *cbItem = new QCheckBox(this);
                    if(query->value(5).toBool())
                    {
                        cbItem->setCheckState(Qt::Checked);
                        flagEdit = true;
                    }
                    connect(cbItem, SIGNAL(stateChanged(int)),this,SLOT(slotStateChanged(int)));
                    ui->tableWidgetViolation->setCellWidget(row,3, cbItem);

                    QCheckBox *cbItem1 = new QCheckBox(this);
                    if((!query->value(5).isNull()) && (!query->value(5).toBool()))
                    {
                        cbItem1->setCheckState(Qt::Checked);
                        flagEdit = true;
                    }
                    connect(cbItem1, SIGNAL(stateChanged(int)),this,SLOT(slotStateChanged(int)));
                    ui->tableWidgetViolation->setCellWidget(row,4, cbItem1);

                    QTableWidgetItem *newItem3 = new QTableWidgetItem();
                    newItem3->setText(query->value(3).toString());
                    newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,5, newItem3);//ID
                    row++;
            }
            if(flagEdit == true)
                ui->pushButtonApply->setEnabled(true);
            else
                ui->pushButtonApply->setEnabled(false);
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
void DEditAudit::slotTextChanged()
{
    ui->pushButtonApply->setEnabled(true);
    ui->pushButtonCancel->setEnabled(true);
}

//=========================================================
void DEditAudit::slotStateChanged(int)
{
    QString sql;
    ui->pushButtonApply->setEnabled(true);
    ui->pushButtonCancel->setEnabled(true);
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    int row = selectedRows[0].row();
    int col = ui->tableWidgetViolation->currentColumn();
    qDebug()<<"row = "<< row;
    qDebug()<<"col = "<< col;
    QObject* obj=QObject::sender();
    if (QCheckBox *cb=qobject_cast<QCheckBox *>(obj))
    {
        QCheckBox *cb1;
        if(col == 3)
            cb1 = reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,4));
        else if(col == 4)
            cb1 = reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,3));
//        qDebug()<<"cb1 = "<<cb1;
        if(cb->isChecked())
        {
            cb1->setCheckState(Qt::Unchecked);
        }
//        else
//            cb1->setCheckState(Qt::Checked);
    }
    if(reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,3))->isChecked())
    {
           sql = "UPDATE \"AnalysisResult\" SET \"CheckExpert\"=\'true\'";
           sql += " WHERE \"ID\"=";
           sql += ui->tableWidgetViolation->item(row,5)->text();
           sql += ";";
     }
     else if(reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,4))->isChecked())
     {
        sql = "UPDATE \"AnalysisResult\" SET \"CheckExpert\"=\'false\'";
        sql += " WHERE \"ID\"=";
        sql += ui->tableWidgetViolation->item(row,5)->text();
        sql += ";";
     }
     else
     {
        sql = "UPDATE \"AnalysisResult\" SET \"CheckExpert\"= NULL";
        sql += " WHERE \"ID\"=";
        sql += ui->tableWidgetViolation->item(row,5)->text();
        sql += ";";
     }
    qDebug()<<"sql="<<sql;
    if(!query->exec(sql))
        qDebug()<<query->lastError().text();
}

//=========================================================
void DEditAudit::slotViewPicture()
{
    QByteArray data;
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    QString sql;
    sql = "SELECT \"Data\" FROM \"Content\" WHERE \"ID_AR\" = ";
    sql += ui->tableWidgetViolation->item(selectedRows[0].row(),5)->text(); sql += ";";

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
const QString DEditAudit::getComment() const
{
    return m_strChecked + ui->textEdit->toPlainText();
}

//=========================================================
void DEditAudit::slotApply()
{
    if(ui->lineEditDoc->text() == "")
    {
        QMessageBox::warning(this, "Внимание","Нельзя поменять статус карточки без прикрепленного экспертного заключения!","Да");
        return;
    }
    QString sql;
    m_strChecked ="";
    for(int row=0; row<ui->tableWidgetViolation->rowCount(); row++)
    {
        m_strChecked += ui->tableWidgetViolation->item(row,0)->text();
        m_strChecked += " - ";
        if(reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,3))->isChecked())
        {
           sql = "UPDATE \"AnalysisResult\" SET \"CheckExpert\"=\'true\'";
           m_strChecked += " подтверждено. ";
        }
        else
        {
            sql = "UPDATE \"AnalysisResult\" SET \"CheckExpert\"=\'false\'";
            m_strChecked += " отклонено. ";
        }
        sql += " WHERE \"ID\"=";
        sql += ui->tableWidgetViolation->item(row,5)->text();
        sql += ";";

//        qDebug()<<"sql="<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();

    }

//    if(ui->lineEditDoc->text().length()>0)
//    {
//        QString fileName;
//        QStringList list = ui->lineEditDoc->text().split("/");
//        fileName = list.at(list.count()-1);
//        qDebug()<<"fileName="<<fileName;
//        sql = "INSERT INTO \"ExpertiseData\"(\"Data\",\"ID_AVP\",\"FileName\") VALUES(:Data,:ID_AVP,:FileName);";
//        query_data->prepare(sql);
//        query_data->bindValue(":Data", getData(ui->lineEditDoc->text()));
//        query_data->bindValue(":ID_AVP", m_idAVP);
//        query_data->bindValue(":FileName", fileName);

//        qDebug()<<"sql="<<sql;
//        if(!query_data->exec())
//            qDebug()<<"ERROR:"<<query_data->lastError().text();
//    }
    m_flagAnswer = true;
    accept();
}

//=========================================================
void DEditAudit::showViolationGroup()
{
    ui->groupBox_2->show();
}

//=========================================================
void DEditAudit::hideViolationGroup()
{
    ui->groupBox_3->hide();
}

//=========================================================
void DEditAudit::hideCurrentExpert()
{
    ui->pushButtonApply->hide();
    ui->pushButton->hide();
    ui->pushButtonOk->show();
    ui->pushButtonReturn->show();
    ui->label_10->show();
    ui->labelCurrentStatus->show();
    ui->label_11->show();
    ui->comboBoxStatus->show();
    ui->pushButtonChageStatus->show();
}

//=========================================================
void DEditAudit::hideExpert()
{
    ui->pushButtonApply->show();
    ui->pushButton->show();
    ui->pushButtonOk->hide();
    ui->pushButtonReturn->hide();
    ui->label_10->hide();
    ui->labelCurrentStatus->hide();
    ui->label_11->hide();
    ui->comboBoxStatus->hide();
    ui->pushButtonChageStatus->hide();
}

//=========================================================
long DEditAudit::idViolation(QString nameViolation)
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
void DEditAudit::slotCancel()
{
    for(int row=0; row<ui->tableWidgetViolation->rowCount(); row++)
    {
        reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,3))->setCheckState(Qt::Unchecked);
        reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,4))->setCheckState(Qt::Unchecked);
    }
    ui->textEdit->clear();
    ui->groupBox_2->setChecked(Qt::Unchecked);
    ui->pushButtonApply->setEnabled(false);
    ui->pushButtonCancel->setEnabled(false);
}

//=========================================================
void DEditAudit::slotCancelExpert()
{
    m_flagAnswer = false;
    accept();
}

//=========================================================
void DEditAudit::slotChangeStatus()
{
    QString sql,tmp;
    try
    {
        sql = "UPDATE \"Task\" SET \"ID_TaskStatus\"=";
        sql += tmp.setNum(getIdTaskStatus(ui->comboBoxStatus->currentText()));
        sql += " WHERE \"ID\"=";
        sql += tmp.setNum(m_idTask); sql += ";";
        qDebug()<<"sql="<<sql;
        if(!query->exec(sql))
        {
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
        else
        {
            ui->labelCurrentStatus->setText(ui->comboBoxStatus->currentText());
            ((MainWindow*)parent())->addRecordJournalJobAVP(1,"Редактирование статуса \"Текущие экспертизы\" АВП",ui->labelNameAVP->text(),m_idAVP);
            ((MainWindow*)parent())->initTableCurrentAudit();
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
    }
}

//=========================================================
void DEditAudit::slotDeleteExpertize()
{
    QString sql;
    QString tmp;
    if(QMessageBox::warning(this,"Внимание","Вы действительно хотите удалить экспертизу?","Да","Нет") == 0)
    {
        try
        {
            sql = "DELETE FROM \"ExpertiseData\" WHERE \"ID_AVP\" = ";
            sql += tmp.setNum(m_idAVP); sql += ";";
            if(!query->exec(sql))
            {
                qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            }
            else
            {
                ui->lineEditDoc->setText("");
                checkExistsExpertise();
            }
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
        }
    }
    else
        return;
}

//=========================================================
void DEditAudit::slotPreviewExpertize()
{
    QByteArray data;
    QString tmp;
    QString sql;
    sql = "SELECT \"Data\",\"FileName\" FROM \"ExpertiseData\" WHERE \"ID_AVP\" = ";
    sql += tmp.setNum(m_idAVP); sql += ";";
//    qDebug()<<"sql = "<<sql;
    if(query->exec(sql))
    {
        if(query->next())
        {
            data = query->value(0).toByteArray();
            QString pathFileName;
            pathFileName = "./"+query->value(1).toString();
            QFile file(pathFileName);
            if(file.open(QFile::WriteOnly))
            {
                file.write(data);
                qDebug()<<"data.size()="<<data.size();
                file.close();
            }
            else
                qDebug()<<"Не могу открыть файл: "+pathFileName;
            if(data.size() != 0)
            {
                QDesktopServices::openUrl(QUrl::fromLocalFile(pathFileName));
            }
        }
        else
            QMessageBox::warning(this, tr("Внимание"),tr("Нет прикрепленной экспертизы!"),tr("Да"));
    }
    else
    {
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
void DEditAudit::slotConfirm()
{
    accept();
}

//=========================================================
void DEditAudit::slotInfo()
{
    dInfoAVP->initTable(m_idAVP);
    dInfoAVP->exec();
//    QString sql,tmp;
//    try
//    {
//        sql = "SELECT * FROM \"JournalJobAVP\";
//        sql += " WHERE \"ID_AVP\"=";
//        sql += tmp.setNum(m_idAVP); sql += ";";
//        qDebug()<<"sql="<<sql;
//        if(!query->exec(sql))
//        {
//            qDebug()<<query->lastError().text();
//            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
//        }
//        else
//        {
//            ((MainWindow*)parent())->addRecordJournalJobAVP(1,"Редактирование статуса на \"Экспертиза\" в закладке \"Текущие экспертизы\" АВП",ui->labelNameAVP->text(),m_idAVP);
//            ((MainWindow*)parent())->initTableCurrentAudit();
//        }
//    }
//    catch(std::exception &e)
//    {
//        qDebug()<<e.what();
//        qDebug()<<query->lastError().text();
//        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
//    }
}

//=========================================================
void DEditAudit::slotReturnAVP()
{
    QString sql,tmp;
    try
    {
        sql = "UPDATE \"Task\" SET \"ID_TaskStatus\"=";
        sql += tmp.setNum(getIdTaskStatus("Экспертиза"));
        sql += " WHERE \"ID\"=";
        sql += tmp.setNum(m_idTask); sql += ";";
        qDebug()<<"sql="<<sql;
        if(!query->exec(sql))
        {
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
        else
        {
//            ui->labelCurrentStatus->setText(ui->comboBoxStatus->currentText());
            ((MainWindow*)parent())->addRecordJournalJobAVP(1,"Редактирование статуса на \"Экспертиза\" в закладке \"Текущие экспертизы\" АВП",ui->labelNameAVP->text(),m_idAVP);
            ((MainWindow*)parent())->initTableCurrentAudit();
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
    }
    reject();
}

//=========================================================
void DEditAudit::setTaskID(long ID)
{
    m_idTask = ID;
}

//=========================================================
bool DEditAudit::checkExistsExpertise()
{
    bool exists = false;
    QByteArray data;
    QString tmp;
    QString sql;
    sql = "SELECT \"Data\",\"FileName\" FROM \"ExpertiseData\" WHERE \"ID_AVP\" = ";
    sql += tmp.setNum(m_idAVP); sql += ";";
//    qDebug()<<"sql = "<<sql;
    if(query->exec(sql))
    {
        if(query->next())
        {
            exists = true;
            ui->label_2->setEnabled(false);
            ui->lineEditDoc->setText(query->value(1).toString());
            ui->lineEditDoc->setEnabled(false);
            ui->label->setEnabled(false);
            ui->toolButton->setEnabled(false);
            ui->toolButtonDeleteExpertise->setEnabled(true);
            ui->toolButtonPreviewExpertise->setEnabled(true);
        }
        else
        {
            exists = false;
            ui->label_2->setEnabled(true);
            ui->lineEditDoc->setText("");
            ui->lineEditDoc->setEnabled(true);
            ui->label->setEnabled(true);
            ui->toolButton->setEnabled(true);
            ui->toolButtonDeleteExpertise->setEnabled(false);
            ui->toolButtonPreviewExpertise->setEnabled(false);
        }
    }
    else
    {
        qDebug()<<query->lastError().text();
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
    }
    return exists;
}

//=========================================================
void DEditAudit::slotApplyViolation()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="",tmp;
    QString s_comment="";
    try
    {
        s_comment ="Таймкод: с " + ui->timeEditBegin->time().toString("HH:mm:ss") + " по "
                + ui->timeEditEnd->time().toString("HH:mm:ss") +". "
                + ui->textEditViolation->toPlainText();
        sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\", \"ID_Violation\",\"TextViolation\",\"Percent\") VALUES(";
        sql += tmp.setNum(m_idAVP); sql += ",";
        sql += tmp.setNum(idViolation(ui->comboBoxViolation->currentText())); sql += ",E\'";
        sql += ((MainWindow*)parent())->cImportData->decode(s_comment); sql += "\',\'100\');";

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
        initTableViolation(m_idAVP);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    hideViolationGroup();
    ui->pushButtonApplyViolation->setEnabled(false);

    ((MainWindow*)parent())->initTableAudit();
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DEditAudit::slotCancelViolation()
{
    ui->textEditViolation->clear();
    ui->lineEditScreenShot->clear();
    ui->pushButtonApplyViolation->setEnabled(false);
    hideViolationGroup();
}

//=========================================================
void DEditAudit::slotAddViolation()
{
    ui->timeEditBegin->setTime(QTime::fromString("00:00:00"));
    ui->timeEditEnd->setTime(QTime::fromString("00:00:00"));
    ui->textEditViolation->clear();
    ui->lineEditScreenShot->clear();
    ui->groupBox_3->show();
    ui->pushButtonApplyViolation->setEnabled(false);
}

//=========================================================
void DEditAudit::slotDeleteViolation()
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
                    sql = "DELETE FROM \"AnalysisResult\" WHERE \"ID\"=";sql += ui->tableWidgetViolation->takeItem(selectedRows[0].row(),5)->text(); sql += ";";
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
        ((MainWindow*)parent())->initTableAudit();
    }
}

//=========================================================
void DEditAudit::slotAtachFile()
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
void DEditAudit::slotAtachAudit()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString fileName ="";
    QString sql;
    fileName = QFileDialog::getOpenFileName(this,
                       QString::fromUtf8("Открыть файл"),
                       QDir::currentPath(),
                       "Все файлы документов (*.doc *.docx *.txt *.pdf *.rtf *.odt *.xlsx *.xls);;Все файлы (*.*)");

    if(fileName != "")
    {
        ui->lineEditDoc->setText(fileName);
        if(ui->lineEditDoc->text().length()>0)
        {
            QString fileName;
            QStringList list = ui->lineEditDoc->text().split("/");
            fileName = list.at(list.count()-1);
            qDebug()<<"fileName="<<fileName;
            sql = "INSERT INTO \"ExpertiseData\"(\"Data\",\"ID_AVP\",\"FileName\") VALUES(:Data,:ID_AVP,:FileName);";
            query_data->prepare(sql);
            query_data->bindValue(":Data", getData(ui->lineEditDoc->text()));
            query_data->bindValue(":ID_AVP", m_idAVP);
            query_data->bindValue(":FileName", fileName);

            qDebug()<<"sql="<<sql;
            if(!query_data->exec())
                qDebug()<<"ERROR:"<<query_data->lastError().text();
            else
                checkExistsExpertise();
        }
    }
    QApplication::restoreOverrideCursor();
}

//=========================================================
int DEditAudit::getIdAnalysisResult()
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
int DEditAudit::getIdTaskStatus(QString taskStatus)
{
    int id = -1;
    QString sql;
    try
    {
        sql = "SELECT \"ID\" FROM \"TaskStatus\" WHERE \"StatusName\"=\'"+ taskStatus +"\';";
        if(query->exec(sql))
        {
            if(query->next())
                id = query->value(0).toInt();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return id;
}

//=========================================================
QByteArray DEditAudit::getData(QString fileName)
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
void DEditAudit::initComboBoxStatus(QString currentStatus)
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
                if((query->value(1).toInt() == 7) ||
                        (query->value(1).toInt() == 6) ||
                        (query->value(1).toInt() == 11) ||
                        (query->value(1).toInt() == 19))
                    ui->comboBoxStatus->addItem(query->value(0).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->labelCurrentStatus->setText(currentStatus);
        ui->comboBoxStatus->setCurrentText(currentStatus);
        if((currentStatus == "Экспертиза") || (currentStatus == "Отложена экспертом") )
        {
            ui->pushButtonOk->setEnabled(false);
            ui->pushButtonReturn->setEnabled(false);
        }
        else
        {
            ui->pushButtonOk->setEnabled(true);
            ui->pushButtonReturn->setEnabled(true);
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditAudit::initComboBoxViolation()
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
void DEditAudit::setNameAVP(QString nameAVP)
{
    ui->labelNameAVP->setText(nameAVP);
}

//=========================================================
void DEditAudit::slotActivated(int)
{
    ui->pushButtonApplyViolation->setEnabled(true);
}

//=========================================================
void DEditAudit::slotTextChangedViolation()
{
    ui->pushButtonApplyViolation->setEnabled(true);
}

//=========================================================
void DEditAudit::slotTextChanged(QString)
{
    ui->pushButtonApplyViolation->setEnabled(true);
}

//=========================================================
void DEditAudit::clear()
{
    ui->lineEditDoc->clear();
}

//=========================================================
void DEditAudit::slotExit()
{
    reject();
}

