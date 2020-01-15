/********************************************************************************
** Form generated from reading UI file 'd_edituser.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_EDITUSER_H
#define UI_D_EDITUSER_H

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

class Ui_DEditUser
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditName;
    QLabel *label_3;
    QLineEdit *lineEditPosition;
    QLabel *label_2;
    QComboBox *comboBoxGroup;
    QSpacerItem *verticalSpacer;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *DEditUser)
    {
        if (DEditUser->objectName().isEmpty())
            DEditUser->setObjectName(QString::fromUtf8("DEditUser"));
        DEditUser->resize(459, 157);
        gridLayout = new QGridLayout(DEditUser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DEditUser);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditName = new QLineEdit(DEditUser);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gridLayout->addWidget(lineEditName, 0, 1, 1, 1);

        label_3 = new QLabel(DEditUser);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        lineEditPosition = new QLineEdit(DEditUser);
        lineEditPosition->setObjectName(QString::fromUtf8("lineEditPosition"));

        gridLayout->addWidget(lineEditPosition, 1, 1, 1, 1);

        label_2 = new QLabel(DEditUser);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        comboBoxGroup = new QComboBox(DEditUser);
        comboBoxGroup->setObjectName(QString::fromUtf8("comboBoxGroup"));

        gridLayout->addWidget(comboBoxGroup, 2, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 1, 1, 1);

        frame = new QFrame(DEditUser);
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


        retranslateUi(DEditUser);
        QObject::connect(pushButtonApply, SIGNAL(clicked()), DEditUser, SLOT(slotApply()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), DEditUser, SLOT(slotCancel()));

        QMetaObject::connectSlotsByName(DEditUser);
    } // setupUi

    void retranslateUi(QDialog *DEditUser)
    {
        DEditUser->setWindowTitle(QApplication::translate("DEditUser", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262 \320\277\320\276\320\273\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
        label->setText(QApplication::translate("DEditUser", "\320\244\320\230\320\236 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217:", nullptr));
        label_3->setText(QApplication::translate("DEditUser", "\320\224\320\276\320\273\320\266\320\275\320\276\321\201\321\202\321\214:", nullptr));
        label_2->setText(QApplication::translate("DEditUser", "\320\223\321\200\321\203\320\277\320\277\320\260:", nullptr));
        pushButtonApply->setText(QApplication::translate("DEditUser", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("DEditUser", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DEditUser: public Ui_DEditUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_EDITUSER_H
