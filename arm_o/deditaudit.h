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
      * @brief idViolation
      * @param nameViolation
      * @return
      */
     long idViolation(QString nameViolation);
     /**
      * @brief hideViolationGroup
      */
     void hideViolationGroup();
     /**
      * @brief showViolationGroup
      */
     void showViolationGroup();
     /**
      * @brief clear
      */
     void clear();
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
    void slotCancelExpert();

private:
    Ui::DEditAudit *ui;
    QSqlQuery *query;
    QSqlQuery *query_data;
    QString m_strChecked;
    int m_idAVP;

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
};

#endif // DEDITAUDIT_H
