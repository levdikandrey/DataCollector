#ifndef DEDITTASK_H
#define DEDITTASK_H
#include <QDialog>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class D_EditTask; }
QT_END_NAMESPACE

class DEditTask: public QDialog
{
        Q_OBJECT
public:
    DEditTask(QWidget *parent = nullptr);
    ~DEditTask();
    void initComboBoxStatus(QString currentStatus);

    void setNameAVP(QString nameAVP);
    void setPercent(QString percent);
    void setComment(QString comment);

    const QString getStatus() const;
    const QString getPercent() const;
    const QString getComment() const;

private slots:
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    Ui::D_EditTask *ui;
    QSqlQuery *query;

    QString m_status;
    QString m_comment;
    QString m_percent;
    QString m_violation;

};

#endif // DEDITTASK_H
