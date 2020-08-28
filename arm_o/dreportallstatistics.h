#ifndef DREPORTALLSTATISTICS_H
#define DREPORTALLSTATISTICS_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DReportAllStatistics;
}

class DReportAllStatistics : public QDialog
{
    Q_OBJECT

public:
    explicit DReportAllStatistics(QWidget *parent = nullptr);
    ~DReportAllStatistics();
    void initTableReportAllStatistics();

private slots:
    void slotMakeReport();
    void slotReview();
    void slotExit();

private:
    void initDialog();
    QSqlQuery *query;
    Ui::DReportAllStatistics *ui;
};

#endif // DREPORTALLSTATISTICS_H
