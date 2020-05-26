#ifndef DJOURNALSESSION_H
#define DJOURNALSESSION_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DJournalSession;
}

class DJournalSession : public QDialog
{
    Q_OBJECT

public:
    explicit DJournalSession(QWidget *parent = nullptr);
    ~DJournalSession();
    void initTableJournalSession();

private slots:
    void slotExit();
    void slotReview();

private:
    void initDialog();
    QSqlQuery *query;
    Ui::DJournalSession *ui;
};

#endif // DJOURNALSESSION_H
