#ifndef DEDITAUDIT_H
#define DEDITAUDIT_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>

namespace Ui {
class DEditAudit;
}

class DEditAudit : public QDialog
{
    Q_OBJECT

public:
    explicit DEditAudit(QWidget *parent = nullptr);
    ~DEditAudit();
     void initTableViolation(long id_avp);
     const QString getComment() const;

public slots:
    void slotTextChanged();
    void slotStateChanged(int);
    void slotViewPicture();
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    Ui::DEditAudit *ui;
    int m_idAVP;
    QSqlQuery *query;
    QSqlQuery *query_data;
    QString m_strChecked;
};

#endif // DEDITAUDIT_H
