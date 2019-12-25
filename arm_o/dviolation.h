#ifndef DVIOLATION_H
#define DVIOLATION_H

#include <QDialog>
#include <QSqlQuery>
#include "daddviolation.h"

namespace Ui {
class DViolation;
}

class DViolation : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DViolation - констркутор класса
     * @param parent - указатель на родителя
     */
    explicit DViolation(QWidget *parent = nullptr);
    /**
      * @brief DViolation - деструктор класса
      */
    ~DViolation();
    /**
     * @brief initTableUser - функцияя заполнения таблицы данными об нарушениях из БД АВП
     */
    void initTableViolation();

private slots:
    /**
     * @brief slotAdd - функция вызывает диалог для добавления нового нарушения в БД АВП
     */
    void slotAdd();
    /**
     * @brief slotDelete - функция удаления нарушени(я/й) из БД АВП
     */
    void slotDelete();
    /**
     * @brief slotEdit - функция вызывает диалог для редактирования наименования нарушения в БД АВП
     */
    void slotEdit();
    /**
     * @brief slotEdit - функция вызывает дмалог для редактирования наименования нарушения в БД АВП (при двойном клике на строке записи в таблице)
     */
    void slotEdit(int, int);
    /**
     * @brief slotExit - функция выхода из диалога
     */
    void slotExit();

private:
    Ui::DViolation *ui;
    /**
     * @brief initGroupList - функция заполнения списка нарушений
     */
    void initViolationList();

    DAddViolation *m_dAddViolation;
    QSqlQuery *query;
    std::map<int, QString> m_violations;
};

#endif // DVIOLATION_H
