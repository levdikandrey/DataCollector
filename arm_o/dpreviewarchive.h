#ifndef DPREVIEWARCHIVE_H
#define DPREVIEWARCHIVE_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class DPreviewArchive;
}

class DPreviewArchive : public QDialog
{
    Q_OBJECT

public:
    explicit DPreviewArchive(QWidget *parent = nullptr);
    ~DPreviewArchive();
    void initTableViolation(long id_avp);

    void setNameAVP(QString nameAVP);
    void setOperatorFIO(QString fio);
    void setExpertFIO(QString fio);
    void setComment(QString comment);

private slots:
    void slotPreviewAudit();
    void slotViewPicture();

private:
    Ui::DPreviewArchive *ui;
    QSqlQuery *query;
    QSqlQuery *query_data;
    long m_idAVP;
};

#endif // DPREVIEWARCHIVE_H
