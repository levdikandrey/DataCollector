#include "dmanualconnecttestdb.h"
#include "ui_d_manualconnecttestdb.h"

//=========================================================
DManualConnectTestDB::DManualConnectTestDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DManualConnectTestDB)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DManualConnectTestDB::~DManualConnectTestDB()
{
    delete ui;
}

//=========================================================
void DManualConnectTestDB::slotExit()
{
    accept();
}
