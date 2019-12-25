#ifndef D_ENTER_H
#define D_ENTER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class D_Enter; }
QT_END_NAMESPACE

class DEnter: public QDialog
{
    Q_OBJECT
public:
    DEnter(QWidget *parent = nullptr);
    QString currentUserName();

private slots:
    void slotEnter();
    void slotCancel();

private:
    Ui::D_Enter *ui;

};

#endif // D_ENTER_H
