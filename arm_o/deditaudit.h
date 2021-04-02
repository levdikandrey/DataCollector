#ifndef DEDITAUDIT_H
#define DEDITAUDIT_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>
#include <QLineEdit>

namespace Ui {
class DEditAudit;
}

/**
 * @brief The DEditAudit class - редактирование карточки эксперта
 */
class DEditAudit : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DEditAudit - конструктор класса
     * @param parent - указатель на родителя
     */
    explicit DEditAudit(QWidget *parent = nullptr);
    /**
      * Деструктор класса.
      */
    ~DEditAudit();
    /**
      * @brief initTableViolation - функция инициализации таблицы данными из БД АВП
      * @param id_avp - идентификатор АВП
      */
     void initTableViolation(long id_avp);
     /**
      * @brief initComboBoxViolation - функция добавления в выпадающей список нарушений из БД АВП
      */
     void initComboBoxViolation();
     /**
      * @brief initComboBoxStatus - функция добавления в выпадающей список статусы для руководителя экспертов из БД АВП
      * @param currentStatus - текущий статус АВП
      */
     void initComboBoxStatus(QString currentStatus);
     /**
      * @brief setNameAVP - функция устанавливает название АВП
      * @param nameAVP - наименование АВП
      */
     void setNameAVP(QString nameAVP);
     /**
      * @brief getComment - функция возвращает текущий комментарий
      * @return - комментарий
      */
     const QString getComment() const;
     /**
      * @brief idViolation -  функция возвращает ID нарушения по его имени
      * @param nameViolation - наименование нарушения
      * @return
      */
     long idViolation(QString nameViolation);
     /**
      * @brief hideViolationGroup - функция прячет группу элементов для доваления нового нарушения
      */
     void hideViolationGroup();
     /**
      * @brief showViolationGroup - функция отображает группу элементов для доваления нового нарушения
      */
     void showViolationGroup();
     /**
      * @brief hideCurrentExpert - функция прячет элементы не нужные для руководителя экспертов
      */
     void hideCurrentExpert();
     /**
      * @brief hideExpert - функция прячет элементы не нужные для экспертf
      */
     void hideExpert();
     /**
      * @brief clear
      */
     void clear();
     /**
      * @brief checkExistsExpertise - функция проверяет сприкреплена ли экспертиза к АВП
      * @return
      */
     bool checkExistsExpertise();
     void setTaskID(long ID);
     bool m_flagAnswer;

public slots:
     /**
     * @brief slotApplyViolation
     */
    void slotApplyViolation();
    /**
     * @brief slotCancelViolation
     */
    void slotCancelViolation();
    /**
     * @brief slotAddViolation
     */
    void slotAddViolation();
    /**
     * @brief slotDeleteViolation
     */
    void slotDeleteViolation();
    /**
     * @brief slotAtachFile
     */
    void slotAtachFile();
    /**
     * @brief slotAtachAudit
     */
    void slotAtachAudit();
    /**
     * @brief slotTextChanged
     */
    void slotTextChanged();
    /**
     * @brief slotStateChanged
     */
    void slotStateChanged(int);
    /**
     * @brief slotViewPicture
     */
    void slotViewPicture();
    /**
     * @brief slotApply - функция применяет внесенные изменения в карточку экспетизы
     */
    void slotApply();
    /**
     * @brief slotCancel
     */
    void slotCancel();
    /**
     * @brief slotExit
     */
    void slotExit();
    /**
     * @brief slotActivated
     */
    void slotActivated(int);
    /**
     * @brief slotTextChangedViolation
     */
    void slotTextChangedViolation();
    /**
     * @brief slotTextChanged
     */
    void slotTextChanged(QString);
    /**
     * @brief slotCancelExpert
     */
    void slotCancelExpert();
    /**
     * @brief slotChangeStatus
     */
    void slotChangeStatus();
    /**
     * @brief slotDeleteExpertize - функция удаляет прикрепленную экспертизу из БД АВП
     */
    void slotDeleteExpertize();
    /**
     * @brief slotPreviewExpertize - функция позволяет просмотреть прикрепленную экспертизу из БД АВП
     */
    void slotPreviewExpertize();
    /**
     * @brief slotConfirm - функция для подтверждения экспертизы руководителем
     */
    void slotConfirm();
    void slotReturnAVP();

private:
    Ui::DEditAudit *ui;
    QSqlQuery *query;
    QSqlQuery *query_data;
    QString m_strChecked;
    int m_idAVP;
    long m_idTask;
    QString m_status;

    /**
     * @brief getIdAnalysisResult
     * @return
     */
    int getIdAnalysisResult();
    /**
     * @brief getData
     * @param fileName
     * @return
     */
    QByteArray getData(QString fileName);
    /**
     * @brief getIdTaskStatus
     * @param taskStatus
     * @return
     */
    int getIdTaskStatus(QString taskStatus);
};

#endif // DEDITAUDIT_H
