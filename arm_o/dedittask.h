#ifndef DEDITTASK_H
#define DEDITTASK_H
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class D_EditTask; }
QT_END_NAMESPACE

class DEditTask: public QDialog
{
        Q_OBJECT
public:
    DEditTask(QWidget *parent = nullptr);

private slots:
    void slotApply();
    void slotCancel();
    void slotExit();

private:
    Ui::D_EditTask *ui;
};

#endif // DEDITTASK_H
