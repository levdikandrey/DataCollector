/********************************************************************************
** Form generated from reading UI file 'd_user.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_USER_H
#define UI_D_USER_H

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

class Ui_DUser
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidgetUser;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonDelete;
    QPushButton *pushButtonEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonExit;

    void setupUi(QDialog *DUser)
    {
        if (DUser->objectName().isEmpty())
            DUser->setObjectName(QString::fromUtf8("DUser"));
        DUser->resize(647, 466);
        gridLayout = new QGridLayout(DUser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidgetUser = new QTableWidget(DUser);
        if (tableWidgetUser->columnCount() < 4)
            tableWidgetUser->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetUser->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetUser->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidgetUser->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidgetUser->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidgetUser->setObjectName(QString::fromUtf8("tableWidgetUser"));
        tableWidgetUser->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetUser->horizontalHeader()->setMinimumSectionSize(0);

        gridLayout->addWidget(tableWidgetUser, 0, 0, 1, 1);

        frame = new QFrame(DUser);
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


        retranslateUi(DUser);
        QObject::connect(pushButtonAdd, SIGNAL(clicked()), DUser, SLOT(slotAdd()));
        QObject::connect(pushButtonDelete, SIGNAL(clicked()), DUser, SLOT(slotDelete()));
        QObject::connect(pushButtonEdit, SIGNAL(clicked()), DUser, SLOT(slotEdit()));
        QObject::connect(pushButtonExit, SIGNAL(clicked()), DUser, SLOT(slotExit()));

        QMetaObject::connectSlotsByName(DUser);
    } // setupUi

    void retranslateUi(QDialog *DUser)
    {
        DUser->setWindowTitle(QApplication::translate("DUser", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\320\270", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetUser->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DUser", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214 (\320\244\320\230\320\236)", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetUser->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DUser", "\320\224\320\276\320\273\320\266\320\275\320\276\321\201\321\202\321\214", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidgetUser->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("DUser", "\320\223\321\200\321\203\320\277\320\277\320\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidgetUser->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("DUser", "ID", nullptr));
        pushButtonAdd->setText(QApplication::translate("DUser", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        pushButtonDelete->setText(QApplication::translate("DUser", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        pushButtonEdit->setText(QApplication::translate("DUser", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        pushButtonExit->setText(QApplication::translate("DUser", "\320\222\321\213\321\205\320\276\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DUser: public Ui_DUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_USER_H
