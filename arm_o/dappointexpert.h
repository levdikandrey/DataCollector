#ifndef DAPPOINTEXPERT_H
#define DAPPOINTEXPERT_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DAppointExpert;
}

class DAppointExpert : public QDialog
{
    Q_OBJECT

public:
    explicit DAppointExpert(QWidget *parent = nullptr);
    ~DAppointExpert();
    const QString getExpertName() const;

public slots:
    void slotApply();
    void slotCancel();

private:
    void initDialog();
    Ui::DAppointExpert *ui;
    QSqlQuery *query;
};

#endif // DAPPOINTEXPERT_H
