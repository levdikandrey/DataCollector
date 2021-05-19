#include "dcloseviolation.h"
#include "ui_d_closeviolation.h"

DCloseViolation::DCloseViolation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DCloseViolation)
{
    ui->setupUi(this);
}

DCloseViolation::~DCloseViolation()
{
    delete ui;
}

const QString DCloseViolation::getComment() const
{
    return ui->textEdit->toPlainText();
}
