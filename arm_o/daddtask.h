#ifndef DADDTASK_H
#define DADDTASK_H

#include <QDialog>
#include <QSqlQuery>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class D_AddTask; }
QT_END_NAMESPACE

class DAddTask: public QDialog
{
        Q_OBJECT
public:
    DAddTask(QWidget *parent = nullptr);
    ~DAddTask();
    /**
     * @brief initAVS - функция заполнения "Выберите АВС"
     */
    void initAVS();
    void initComboBoxUser();
    void initComboBoxPriority();
    void initComboBoxStatus();
    void initTableListAVP(int numberPage = 1, long idAVS = -1, int state = -1);
    void clearLineFindAVP();

    const QString getUserFIO() const;
    const QString getStatusName() const;
    const QString getNamePriority() const;
    const std::map<long,QString> getListAVP() const;

private slots:
    void slotAdd();
    void slotDelete();
    void slotApply();
    void slotCancel();
    void slotExit();
    void slotPrevious();
    void slotNext();
    void slotChangeNumberPage();
    void slotFindAVP();
    void slotTextChanged(const QString&);

    void slotRBViolation(bool);
    void slotRBChecked(bool);
    void slotRBAll(bool);
    /**
     * @brief slotSelectAVS
     * @param avs
     */
    void slotSelectAVS(QString);

private:
    Ui::D_AddTask *ui;
    QSqlQuery *query;
    std::map<long,QString> listAVP;
    QTimer m_timer;

    int countAVP(long idAVS = -1);
    long idAVS(QString nameAVS);
    bool isExistAVP(long idAVS = -1);

    int m_countAVP;
    int m_currentNumberPage;
    int m_currentState;
    int m_currentIdAVS;
    int m_countCurrentAVP;
};

#endif // DADDTASK_H
