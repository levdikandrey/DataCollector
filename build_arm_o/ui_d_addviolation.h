/********************************************************************************
** Form generated from reading UI file 'd_addviolation.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_ADDVIOLATION_H
#define UI_D_ADDVIOLATION_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DAddViolation
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEditName;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;
    QLabel *label;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *DAddViolation)
    {
        if (DAddViolation->objectName().isEmpty())
            DAddViolation->setObjectName(QString::fromUtf8("DAddViolation"));
        DAddViolation->resize(454, 111);
        gridLayout = new QGridLayout(DAddViolation);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEditName = new QLineEdit(DAddViolation);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gridLayout->addWidget(lineEditName, 0, 1, 1, 1);

        frame = new QFrame(DAddViolation);
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


        gridLayout->addWidget(frame, 2, 0, 1, 2);

        label = new QLabel(DAddViolation);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);


        retranslateUi(DAddViolation);
        QObject::connect(pushButtonApply, SIGNAL(clicked()), DAddViolation, SLOT(slotApply()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), DAddViolation, SLOT(slotCancel()));

        QMetaObject::connectSlotsByName(DAddViolation);
    } // setupUi

    void retranslateUi(QDialog *DAddViolation)
    {
        DAddViolation->setWindowTitle(QApplication::translate("DAddViolation", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\275\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217", nullptr));
        pushButtonApply->setText(QApplication::translate("DAddViolation", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("DAddViolation", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        label->setText(QApplication::translate("DAddViolation", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265 \320\275\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DAddViolation: public Ui_DAddViolation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_ADDVIOLATION_H
