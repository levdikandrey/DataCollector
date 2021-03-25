#ifndef DMANUALCONNECTTESTDB_H
#define DMANUALCONNECTTESTDB_H

#include <QDialog>

namespace Ui {
class DManualConnectTestDB;
}

class DManualConnectTestDB : public QDialog
{
    Q_OBJECT

public:
    explicit DManualConnectTestDB(QWidget *parent = nullptr);
    ~DManualConnectTestDB();

private slots:
    void slotExit();

private:
    Ui::DManualConnectTestDB *ui;
};

#endif // DMANUALCONNECTTESTDB_H
