#ifndef DINFOAVP_H
#define DINFOAVP_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DInfoAVP;
}

class DInfoAVP : public QDialog
{
    Q_OBJECT

public:
    explicit DInfoAVP(QWidget *parent = nullptr);
    ~DInfoAVP();
    void initTable(long id_avp);
    void initDialog();

private:
    Ui::DInfoAVP *ui;
    QSqlQuery *query;
};

#endif // DINFOAVP_H
