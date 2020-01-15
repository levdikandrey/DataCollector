/********************************************************************************
** Form generated from reading UI file 'd_importdata.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_IMPORTDATA_H
#define UI_D_IMPORTDATA_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_D_ImportDataOutExel
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonExit;
    QListWidget *listWidget;
    QProgressBar *progressBar;

    void setupUi(QDialog *D_ImportDataOutExel)
    {
        if (D_ImportDataOutExel->objectName().isEmpty())
            D_ImportDataOutExel->setObjectName(QString::fromUtf8("D_ImportDataOutExel"));
        D_ImportDataOutExel->resize(673, 658);
        gridLayout = new QGridLayout(D_ImportDataOutExel);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame = new QFrame(D_ImportDataOutExel);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(693, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonExit = new QPushButton(frame);
        pushButtonExit->setObjectName(QString::fromUtf8("pushButtonExit"));
        pushButtonExit->setMinimumSize(QSize(0, 30));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../icons/log_logout_door_1563.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonExit->setIcon(icon);
        pushButtonExit->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonExit);


        gridLayout->addWidget(frame, 3, 0, 1, 1);

        listWidget = new QListWidget(D_ImportDataOutExel);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setSelectionBehavior(QAbstractItemView::SelectItems);

        gridLayout->addWidget(listWidget, 1, 0, 1, 1);

        progressBar = new QProgressBar(D_ImportDataOutExel);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(1);

        gridLayout->addWidget(progressBar, 2, 0, 1, 1);


        retranslateUi(D_ImportDataOutExel);
        QObject::connect(pushButtonExit, SIGNAL(clicked()), D_ImportDataOutExel, SLOT(slotExit()));

        QMetaObject::connectSlotsByName(D_ImportDataOutExel);
    } // setupUi

    void retranslateUi(QDialog *D_ImportDataOutExel)
    {
        D_ImportDataOutExel->setWindowTitle(QApplication::translate("D_ImportDataOutExel", "\320\227\320\260\320\263\321\200\321\203\320\267\320\272\320\260 \320\264\320\260\320\275\320\275\321\213\321\205 \320\262 \320\221\320\224 \320\220\320\222\320\237", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonExit->setToolTip(QApplication::translate("D_ImportDataOutExel", "\320\222\321\213\321\205\320\276\320\264 \320\270\320\267 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\321\217", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonExit->setText(QApplication::translate("D_ImportDataOutExel", "\320\222\321\213\321\205\320\276\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class D_ImportDataOutExel: public Ui_D_ImportDataOutExel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_IMPORTDATA_H
