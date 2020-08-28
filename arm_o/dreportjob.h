#ifndef DREPORTJOB_H
#define DREPORTJOB_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DReportJob;
}

class DReportJob : public QDialog
{
    Q_OBJECT

public:
    explicit DReportJob(QWidget *parent = nullptr);
    ~DReportJob();
    void initTableReportJob();

private slots:
    void slotMakeReport();
    void slotReview();
    void slotExit();

private:
    void initDialog();

private:
    QSqlQuery *query;
    Ui::DReportJob *ui;
};

#endif // DREPORTJOB_H
