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

    const QString userName() const;
    const QString position() const;

    void setUserName(QString name);
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
