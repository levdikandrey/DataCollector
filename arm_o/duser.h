#ifndef DUSER_H
#define DUSER_H

#include <QDialog>
#include <QSqlQuery>
#include "dadduser.h"
#include "dedituser.h"

namespace Ui {
class DUser;
}
/**
 * @brief The DUser class - класс диалога для работы с пользователями
 */
class DUser : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DUser - констркутор класса
     * @param parent - указатель на родителя
     */
    explicit DUser(QWidget *parent = nullptr);
    /**
      * @brief DUser - деструктор класса
      */
    ~DUser();
    /**
     * @brief initTableUser - функцияя заполнения таблицы данными о пользователях из БД АВП
     */
    void initTableUser();

private slots:
    /**
     * @brief slotAdd - функция вызывает диалог для добавления нового пользователя в БД АВП
     */
    void slotAdd();
    /**
     * @brief slotDelete - функция удаления пользовател(я/ей) из БД АВП
     */
    void slotDelete();
    /**
     * @brief slotEdit - функция вызывает диалог для редактирования параметров пользователей в БД АВП
     */
    void slotEdit();
    /**
     * @brief slotEdit - функция вызывает дмалог для редактирования параметров пользователей в БД АВП (при двойном клике на строке записи в таблице)
     */
    void slotEdit(int, int);
    /**
     * @brief slotExit - функция выхода из диалога
     */
    void slotExit();

private:
    /**
     * @brief initGroupList - функция заполнения списка групп
     */
    void initGroupList();

    QSqlQuery *query;
    DAddUser *m_dAddUser;
    DEditUser *m_dEditUser;
    Ui::DUser *ui;
    std::map<int, QString> m_groups;
};

#endif // DUSER_H
