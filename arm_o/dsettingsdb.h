#ifndef DSETTINGSDB_H
#define DSETTINGSDB_H

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QDir>

namespace Ui {
class DSettingsDB;
}

class DSettingsDB : public QDialog
{
    Q_OBJECT

public:
    explicit DSettingsDB(QWidget *parent = nullptr);
    ~DSettingsDB();
    void initDialog();

private slots:
    void slotApply();
    void slotCancel();

private:
    Ui::DSettingsDB *ui;
    QSettings *settings;
};

#endif // DSETTINGSDB_H
