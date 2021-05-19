#ifndef DCLOSEVIOLATION_H
#define DCLOSEVIOLATION_H

#include <QDialog>

namespace Ui {
class DCloseViolation;
}

class DCloseViolation : public QDialog
{
    Q_OBJECT

public:
    explicit DCloseViolation(QWidget *parent = nullptr);
    ~DCloseViolation();
    const QString getComment() const;

private:
    Ui::DCloseViolation *ui;
};

#endif // DCLOSEVIOLATION_H
