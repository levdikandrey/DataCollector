#include "daddviolation.h"
#include "ui_d_addviolation.h"

#include <QMessageBox>
//=========================================================
DAddViolation::DAddViolation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DAddViolation)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DAddViolation::~DAddViolation()
{
    delete ui;
}

//=========================================================
const QString DAddViolation::violation() const
{
    return ui->lineEditName->text();
}

//=========================================================
const QString DAddViolation::getLawText() const
{
    return ui->plainTextEdit->toPlainText();
}

//=========================================================
int DAddViolation::getType() const
{
    int type = 2;
    if(ui->comboBox->currentText() == "Нарушения")
        type = 1;
    return type;
}

//=========================================================
void DAddViolation::setViolation(QString violation)
{
    ui->lineEditName->setText(violation);
}

//=========================================================
void DAddViolation::setTitle(QString title)
{
    setWindowTitle(title);
}

//=========================================================
void DAddViolation::showItem()
{
    ui->label_2->show();
    ui->comboBox->show();
    ui->groupBox->show();
}

//=========================================================
void DAddViolation::showItemEdit()
{
    ui->label_2->show();
    ui->comboBox->hide();
    ui->groupBox->show();
}

//=========================================================
void DAddViolation::hideItem()
{
    ui->label_2->hide();
    ui->comboBox->hide();
    ui->groupBox->hide();
}

//=========================================================
void DAddViolation::slotApply()
{
    if(ui->lineEditName->text()=="")
    {
        QMessageBox::warning(this, tr("Внимание"),tr("Введите наименование нарушения!"),tr("Да"));
        return;
    }
    accept();
}

//=========================================================
void DAddViolation::slotCancel()
{
    reject();
}
