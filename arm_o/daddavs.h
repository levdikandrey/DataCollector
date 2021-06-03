#ifndef DADDAVS_H
#define DADDAVS_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DAddAVS;
}

class DAddAVS : public QDialog
{
    Q_OBJECT

public:
    explicit DAddAVS(QWidget *parent = nullptr);
    ~DAddAVS();
    QString getName();
    QString getURL();

private slots:
    void slotApply();

private:
    Ui::DAddAVS *ui;
    QSqlQuery *query;

};

#endif // DADDAVS_H
