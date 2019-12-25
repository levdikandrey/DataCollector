#ifndef DADDUSER_H
#define DADDUSER_H

#include <QDialog>

namespace Ui {
class DAddUser;
}

class DAddUser : public QDialog
{
    Q_OBJECT

public:
    explicit DAddUser(QWidget *parent = nullptr);
    ~DAddUser();
    const QString userName() const;
    const QString range() const;
    int idGroup();
    void addGroup(std::map<int,QString> groups);

public slots:
    void slotApply();
    void slotCancel();

private:
    Ui::DAddUser *ui;
    std::map<int,QString> m_groups;
};

#endif // DADDUSER_H
