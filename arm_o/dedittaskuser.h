#ifndef DEDITTASKUSER_H
#define DEDITTASKUSER_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class D_EditTaskUser; }
QT_END_NAMESPACE

class DEditTaskUser: public QDialog
{
        Q_OBJECT
public:
    DEditTaskUser(QWidget *parent = nullptr);
    ~DEditTaskUser();
    void initComboBoxUser(QString currentUser);
    void initComboBoxPriority(QString currentPriority);
    void initComboBoxStatus(QString currentStatus);
    void initTableViolation(long id_avp);
    void initDateRealization(long id_avp);

    void setNameAVP(QString nameAVP);
    void setPercent(QString percent);
    void setComment(QString comment);

    const QString getNameUser() const;
    const QString getPriority() const;
    const QString getStatus() const;
    const QString getPercent() const;
    const QString getComment() const;
    const QString getDateRealization() const;
    QTableWidget* getViolations() const;

private slots:
    void slotViewPicture();
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    Ui::D_EditTaskUser *ui;
    QSqlQuery *query;

    QString m_user;
    QString m_status;
    QString m_priority;
    QString m_comment;
    QString m_percent;
    QString m_violation;

    int m_idAVP;
};

#endif // DEDITTASKUSER_H
