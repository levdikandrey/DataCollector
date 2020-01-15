/********************************************************************************
** Form generated from reading UI file 'd_violation.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_VIOLATION_H
#define UI_D_VIOLATION_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_DViolation
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidgetViolation;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonDelete;
    QPushButton *pushButtonEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonExit;

    void setupUi(QDialog *DViolation)
    {
        if (DViolation->objectName().isEmpty())
            DViolation->setObjectName(QString::fromUtf8("DViolation"));
        DViolation->resize(644, 464);
        gridLayout = new QGridLayout(DViolation);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidgetViolation = new QTableWidget(DViolation);
        if (tableWidgetViolation->columnCount() < 2)
            tableWidgetViolation->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetViolation->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetViolation->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidgetViolation->setObjectName(QString::fromUtf8("tableWidgetViolation"));
        tableWidgetViolation->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetViolation->horizontalHeader()->setMinimumSectionSize(0);

        gridLayout->addWidget(tableWidgetViolation, 0, 0, 1, 1);

        frame = new QFrame(DViolation);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonAdd = new QPushButton(frame);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
        pushButtonAdd->setMinimumSize(QSize(120, 33));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/add.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonAdd->setIcon(icon);
        pushButtonAdd->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonAdd);

        pushButtonDelete = new QPushButton(frame);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));
        pushButtonDelete->setMinimumSize(QSize(120, 33));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/delete.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonDelete->setIcon(icon1);
        pushButtonDelete->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonDelete);

        pushButtonEdit = new QPushButton(frame);
        pushButtonEdit->setObjectName(QString::fromUtf8("pushButtonEdit"));
        pushButtonEdit->setMinimumSize(QSize(120, 33));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/edit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonEdit->setIcon(icon2);
        pushButtonEdit->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonEdit);

        horizontalSpacer = new QSpacerItem(103, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonExit = new QPushButton(frame);
        pushButtonExit->setObjectName(QString::fromUtf8("pushButtonExit"));
        pushButtonExit->setMinimumSize(QSize(120, 33));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonExit->setIcon(icon3);
        pushButtonExit->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonExit);


        gridLayout->addWidget(frame, 1, 0, 1, 1);


        retranslateUi(DViolation);
        QObject::connect(pushButtonAdd, SIGNAL(clicked()), DViolation, SLOT(slotAdd()));
        QObject::connect(pushButtonDelete, SIGNAL(clicked()), DViolation, SLOT(slotDelete()));
        QObject::connect(pushButtonEdit, SIGNAL(clicked()), DViolation, SLOT(slotEdit()));
        QObject::connect(pushButtonExit, SIGNAL(clicked()), DViolation, SLOT(slotExit()));

        QMetaObject::connectSlotsByName(DViolation);
    } // setupUi

    void retranslateUi(QDialog *DViolation)
    {
        DViolation->setWindowTitle(QApplication::translate("DViolation", "\320\241\320\277\321\200\320\260\320\262\320\276\321\207\320\275\320\270\320\272 \320\275\320\260\321\200\321\203\321\210\320\265\320\275\320\270\320\271", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetViolation->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DViolation", "\320\235\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetViolation->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DViolation", "ID", nullptr));
        pushButtonAdd->setText(QApplication::translate("DViolation", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        pushButtonDelete->setText(QApplication::translate("DViolation", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        pushButtonEdit->setText(QApplication::translate("DViolation", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        pushButtonExit->setText(QApplication::translate("DViolation", "\320\222\321\213\321\205\320\276\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DViolation: public Ui_DViolation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_VIOLATION_H
