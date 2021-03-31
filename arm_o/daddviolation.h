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
    void showItem();
    void showItemEdit();
    void hideItem();

    void setViolation(QString violation);
    void setTitle(QString title);

    int getType() const;
    const QString violation() const;
    const QString getLawText() const;

public slots:
    void slotApply();
    void slotCancel();

private:
    Ui::DAddViolation *ui;
};

#endif // DADDVIOLATION_H
