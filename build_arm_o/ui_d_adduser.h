/********************************************************************************
** Form generated from reading UI file 'd_adduser.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_ADDUSER_H
#define UI_D_ADDUSER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DAddUser
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditName;
    QLabel *label_3;
    QLineEdit *lineEditRange;
    QLabel *label_2;
    QComboBox *comboBoxGroup;
    QSpacerItem *verticalSpacer;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *DAddUser)
    {
        if (DAddUser->objectName().isEmpty())
            DAddUser->setObjectName(QString::fromUtf8("DAddUser"));
        DAddUser->resize(458, 157);
        gridLayout = new QGridLayout(DAddUser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DAddUser);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditName = new QLineEdit(DAddUser);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gridLayout->addWidget(lineEditName, 0, 1, 1, 1);

        label_3 = new QLabel(DAddUser);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        lineEditRange = new QLineEdit(DAddUser);
        lineEditRange->setObjectName(QString::fromUtf8("lineEditRange"));

        gridLayout->addWidget(lineEditRange, 1, 1, 1, 1);

        label_2 = new QLabel(DAddUser);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        comboBoxGroup = new QComboBox(DAddUser);
        comboBoxGroup->setObjectName(QString::fromUtf8("comboBoxGroup"));

        gridLayout->addWidget(comboBoxGroup, 2, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 1, 1, 1);

        frame = new QFrame(DAddUser);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(161, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonApply = new QPushButton(frame);
        pushButtonApply->setObjectName(QString::fromUtf8("pushButtonApply"));
        pushButtonApply->setMinimumSize(QSize(120, 33));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/ok.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonApply->setIcon(icon);
        pushButtonApply->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonApply);

        pushButtonCancel = new QPushButton(frame);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setMinimumSize(QSize(120, 33));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/cancel.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonCancel->setIcon(icon1);
        pushButtonCancel->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonCancel);


        gridLayout->addWidget(frame, 4, 0, 1, 2);


        retranslateUi(DAddUser);
        QObject::connect(pushButtonApply, SIGNAL(clicked(bool)), DAddUser, SLOT(slotApply()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), DAddUser, SLOT(slotCancel()));

        QMetaObject::connectSlotsByName(DAddUser);
    } // setupUi

    void retranslateUi(QDialog *DAddUser)
    {
        DAddUser->setWindowTitle(QApplication::translate("DAddUser", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\275\320\276\320\262\320\276\320\263\320\276 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
        label->setText(QApplication::translate("DAddUser", "\320\244\320\230\320\236 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217:", nullptr));
        label_3->setText(QApplication::translate("DAddUser", "\320\224\320\276\320\273\320\266\320\275\320\276\321\201\321\202\321\214:", nullptr));
        label_2->setText(QApplication::translate("DAddUser", "\320\223\321\200\321\203\320\277\320\277\320\260:", nullptr));
        pushButtonApply->setText(QApplication::translate("DAddUser", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("DAddUser", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DAddUser: public Ui_DAddUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_ADDUSER_H
