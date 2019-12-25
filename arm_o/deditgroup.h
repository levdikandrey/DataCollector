#ifndef DEDITGROUP_H
#define DEDITGROUP_H

#include <QDialog>

namespace Ui {
class DEditGroup;
}

class DEditGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DEditGroup(QWidget *parent = nullptr);
    ~DEditGroup();
    QString groupName();
    QString groupCategory();
    void setGroupName(QString name);

public slots:
    void slotApply();
    void slotCancel();

private:
    Ui::DEditGroup *ui;
};

#endif // DEDITGROUP_H
