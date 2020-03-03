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
     void initComboBoxViolation();
     const QString getComment() const;
     long idViolation(QString nameViolation);
     void hideViolationGroup();
     void showViolationGroup();

public slots:
    void slotApplyViolation();
    void slotCancelViolation();
    void slotAddViolation();
    void slotDeleteViolation();
    void slotAtachFile();
    void slotTextChanged();
    void slotStateChanged(int);
    void slotViewPicture();
    void slotApply();
    void slotCancel();
    void slotExit();

    void slotActivated(int);
    void slotTextChangedViolation();
    void slotTextChanged(QString);

private:
    Ui::DEditAudit *ui;
    QSqlQuery *query;
    QSqlQuery *query_data;
    QString m_strChecked;

    int m_idAVP;
    int getIdAnalysisResult();
    QByteArray getData(QString fileName);
};

#endif // DEDITAUDIT_H
