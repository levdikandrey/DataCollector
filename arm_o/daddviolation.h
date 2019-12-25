#ifndef DADDVIOLATION_H
#define DADDVIOLATION_H

#include <QDialog>

namespace Ui {
class DAddViolation;
}

class DAddViolation : public QDialog
{
    Q_OBJECT

public:
    explicit DAddViolation(QWidget *parent = nullptr);
    ~DAddViolation();
    const QString violation() const;

public slots:
    void slotApply();
    void slotCancel();

private:
    Ui::DAddViolation *ui;
};

#endif // DADDVIOLATION_H
