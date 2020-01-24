#ifndef DEDITTASK_H
#define DEDITTASK_H
#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class D_EditTask; }
QT_END_NAMESPACE

class DEditTask: public QDialog
{
        Q_OBJECT
public:
    DEditTask(QWidget *parent = nullptr);
    ~DEditTask();
    void initTableViolation(long id_avp);
    void initComboBoxStatus(QString currentStatus);
    void initComboBoxViolation();

    void setNameAVP(QString nameAVP);
    void setPercent(QString percent);
    void setComment(QString comment);

    const QString getStatus() const;
    const QString getPercent() const;
    const QString getComment() const;
    QTableWidget* getViolations() const;

    long idViolation(QString nameViolation);
    void hideViolationGroup();
    void showViolationGroup();

private slots:
    void slotViewPicture();
    void slotApplyViolation();
    void slotCancelViolation();
    void slotAddViolation();
    void slotDeleteViolation();
    void slotAtachFile();
    void slotApply();
    void slotCancel();
    void slotExit();

    void slotStatusActivated(int);
    void slotValueChanged(int);

    void slotActivated(int);
    void slotTextChanged();
    void slotTextChanged(QString);

private:
    Ui::D_EditTask *ui;
    QSqlQuery *query;
    QSqlQuery *query_data;

    QString m_status;
    QString m_comment;
    QString m_percent;
    QString m_violation;

    int m_idAVP;
    int getIdAnalysisResult();
    QByteArray getData(QString fileName);
};

#endif // DEDITTASK_H
