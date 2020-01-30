#ifndef DADDAVP_H
#define DADDAVP_H

#include <QDialog>
#include <QSqlQuery>
#include "cimportdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class D_AddAVP; }
QT_END_NAMESPACE

class DAddAVP: public QDialog
{
    Q_OBJECT
public:
    DAddAVP(QWidget *parent = nullptr);
    ~DAddAVP();
    /**
     * @brief initAVS - функция заполнения "Выберите АВС"
     */
    void initAVS();

    const QString getNameAVS() const;
    const QString getNameAVP() const;
    const QString getNameOriginal() const;
    const QString getURL_AVS() const;
    const QString getURL_AVP() const;
    const QString getFormAVP() const;
    const QString getRubricAVP() const;
    const QString getFilmMaker() const;
    const QString getAge() const;
    const QString getYearOfRelease() const;
    const QString getDuration() const;

private slots:
    void slotActivated(int item);
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    CImportData *m_cImportData;
    QSqlQuery *query;
    Ui::D_AddAVP *ui;
    std::map<int, QString> listAVS;

    bool addAVP();
};

#endif // DADDAVP_H
