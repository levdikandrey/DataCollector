#ifndef DEDITTASKUSER_H
#define DEDITTASKUSER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class D_EditTaskUser; }
QT_END_NAMESPACE

class DEditTaskUser: public QDialog
{
        Q_OBJECT
public:
    DEditTaskUser(QWidget *parent = nullptr);

private slots:
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    Ui::D_EditTaskUser *ui;
};

#endif // DEDITTASKUSER_H
