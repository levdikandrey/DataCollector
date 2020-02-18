#ifndef DEDITUSER_H
#define DEDITUSER_H

#include <QDialog>

namespace Ui {
class DEditUser;
}

class DEditUser : public QDialog
{
    Q_OBJECT

public:
    explicit DEditUser(QWidget *parent = nullptr);
    ~DEditUser();

    const QString lastName() const;
    const QString firstName() const;
    const QString middleName() const;
    const QString position() const;

    void setLastName(QString name);
    void setFirstName(QString name);
    void setMiddleName(QString name);
    void setPosition(QString position);

    int idGroup();
    void addGroup(std::map<int,QString> groups, QString currentGroup="");

public slots:
    void slotApply();
    void slotCancel();

private:
    Ui::DEditUser *ui;
    std::map<int,QString> m_groups;
};

#endif // DEDITUSER_H
