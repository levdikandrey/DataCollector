#ifndef DEDITAVP_H
#define DEDITAVP_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DEditAVP;
}

class DEditAVP : public QDialog
{
    Q_OBJECT

public:
    explicit DEditAVP(QWidget *parent = nullptr);
    ~DEditAVP();
    void initAVS(uint64_t idAVP);

    const QString getNameAVP() const;
    const QString getNameOriginal() const;
    const QString getURL_AVP() const;
    const QString getRubricAVP() const;
    const QString getFilmMaker() const;
    const QString getAge() const;
    const QString getYearOfRelease() const;
    const QString getDuration() const;

private slots:
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    Ui::DEditAVP *ui;
    QSqlQuery *query;
    uint64_t m_idAVP;
};

#endif // DEDITAVP_H
