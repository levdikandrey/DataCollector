#ifndef DREPORTALLVIOLATIONAVP_H
#define DREPORTALLVIOLATIONAVP_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DReportAllViolationAVP;
}

class DReportAllViolationAVP : public QDialog
{
    Q_OBJECT

public:
    explicit DReportAllViolationAVP(QWidget *parent = nullptr);
    ~DReportAllViolationAVP();
    void initTableReportViolationAVP();

private slots:
    void slotMakeReport();
    void slotReview();
    void slotExit();

private:
    void initDialog();
    QSqlQuery *query;
    QSqlQuery *query1;
    Ui::DReportAllViolationAVP *ui;
};

#endif // DREPORTALLVIOLATIONAVP_H
