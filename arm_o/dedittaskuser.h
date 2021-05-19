#ifndef DEDITTASKUSER_H
#define DEDITTASKUSER_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>
#include "dinfoavp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class D_EditTaskUser; }
QT_END_NAMESPACE

/**
 * @brief DEditTaskUser - класс для редактирования задачи в закладке "Текущие задачи"
 */
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
    void initTableAttribute(long id_avp);

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

    bool flagEditOperator;
    bool flagEditStatus;
    bool flagEditPriority;
    bool flagEditPercent;
    bool flagEditComment;
    bool flagEditDate;

private slots:
    void slotViewPicture();
    void slotApply();
    void slotCancel();
    void slotExit();
    void slotDateChanged(const QDate &);
    void slotPreviewExpertise();
    /**
     * @brief slotFree - функция устанавливает признак АВП в состояние - бесплатный. В БД значение 0.
     */
    void slotFree(bool);
    /**
     * @brief slotPaid - функция устанавливает признак АВП в состояние - платный. В БД значение 1.
     */
    void slotPaid(bool);
    /**
     * @brief slotPurchased - функция устанавливает признак АВП в состояние - куплен. В БД значение 2.
     */
    void slotPurchased(bool);
    /**
     * @brief slotRent - функция устанавливает признак АВП в состояние - аренда. В БД значение 3.
     */
    void slotRent(bool);
    /**
     * @brief slotValueChangedDay - функция устанавливает количество дней аренды АВП.
     */
    void slotValueChangedDay(int);
    void slotInfo();


private:
    Ui::D_EditTaskUser *ui;
    QSqlQuery *query;

    QString m_user;
    QString m_status;
    QString m_priority;
    QString m_comment;
    QString m_percent;
    QString m_violation;

    DInfoAVP *dInfoAVP;

    int m_idAVP;
};

#endif // DEDITTASKUSER_H
