#ifndef D_ADDGROUP_H
#define D_ADDGROUP_H

#include <QDialog>

namespace Ui {
class D_AddGroup;
}

class D_AddGroup : public QDialog
{
    Q_OBJECT

public:
    explicit D_AddGroup(QWidget *parent = nullptr);
    ~D_AddGroup();
    QString groupName();
    QString groupCategory();

public slots:
    void slotApply();
    void slotCancel();

private:
    Ui::D_AddGroup *ui;
};

#endif // D_ADDGROUP_H
