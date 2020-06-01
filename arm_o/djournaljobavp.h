#ifndef DJOURNALJOBAVP_H
#define DJOURNALJOBAVP_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DJournalJobAVP;
}

class DJournalJobAVP : public QDialog
{
    Q_OBJECT

public:
    explicit DJournalJobAVP(QWidget *parent = nullptr);
    ~DJournalJobAVP();
    void initTableJournalSession();

private slots:
    void slotExit();
    void slotReview();

private:
    void initDialog();
    QSqlQuery *query;
    Ui::DJournalJobAVP *ui;
};

#endif // DJOURNALJOBAVP_H
