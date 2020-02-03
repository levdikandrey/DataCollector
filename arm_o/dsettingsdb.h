#ifndef DSETTINGSDB_H
#define DSETTINGSDB_H

#include <QDialog>

namespace Ui {
class DSettingsDB;
}

class DSettingsDB : public QDialog
{
    Q_OBJECT

public:
    explicit DSettingsDB(QWidget *parent = nullptr);
    ~DSettingsDB();

private slots:
    void slotApply();
    void slotCancel();

private:
    Ui::DSettingsDB *ui;
};

#endif // DSETTINGSDB_H
