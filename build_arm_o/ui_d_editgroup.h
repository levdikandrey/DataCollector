/********************************************************************************
** Form generated from reading UI file 'd_editgroup.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_EDITGROUP_H
#define UI_D_EDITGROUP_H

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

class Ui_DEditGroup
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditName;
    QLabel *label_2;
    QComboBox *comboBox;
    QSpacerItem *verticalSpacer;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *DEditGroup)
    {
        if (DEditGroup->objectName().isEmpty())
            DEditGroup->setObjectName(QString::fromUtf8("DEditGroup"));
        DEditGroup->resize(456, 131);
        gridLayout = new QGridLayout(DEditGroup);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DEditGroup);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditName = new QLineEdit(DEditGroup);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gridLayout->addWidget(lineEditName, 0, 1, 1, 1);

        label_2 = new QLabel(DEditGroup);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        comboBox = new QComboBox(DEditGroup);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout->addWidget(comboBox, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 1, 1, 1);

        frame = new QFrame(DEditGroup);
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


        gridLayout->addWidget(frame, 3, 0, 1, 2);

        QWidget::setTabOrder(lineEditName, comboBox);
        QWidget::setTabOrder(comboBox, pushButtonApply);
        QWidget::setTabOrder(pushButtonApply, pushButtonCancel);

        retranslateUi(DEditGroup);
        QObject::connect(pushButtonApply, SIGNAL(clicked()), DEditGroup, SLOT(slotApply()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), DEditGroup, SLOT(slotCancel()));

        QMetaObject::connectSlotsByName(DEditGroup);
    } // setupUi

    void retranslateUi(QDialog *DEditGroup)
    {
        DEditGroup->setWindowTitle(QApplication::translate("DEditGroup", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\263\321\200\321\203\320\277\320\277\321\213", nullptr));
        label->setText(QApplication::translate("DEditGroup", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\263\321\200\321\203\320\277\320\277\321\213:", nullptr));
        label_2->setText(QApplication::translate("DEditGroup", "\320\232\320\260\321\202\320\265\320\263\320\276\321\200\320\270\321\217 \320\263\321\200\321\203\320\277\320\277\321\213:", nullptr));
        comboBox->setItemText(0, QApplication::translate("DEditGroup", "\320\236\320\277\320\265\321\200\320\260\321\202\320\276\321\200\321\213", nullptr));
        comboBox->setItemText(1, QApplication::translate("DEditGroup", "\320\240\321\203\320\272\320\276\320\262\320\276\320\264\321\201\321\202\320\262\320\276", nullptr));
        comboBox->setItemText(2, QApplication::translate("DEditGroup", "\320\220\320\264\320\274\320\270\320\275\320\270\321\201\321\202\321\200\320\260\321\202\320\276\321\200\321\213", nullptr));

        pushButtonApply->setText(QApplication::translate("DEditGroup", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("DEditGroup", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DEditGroup: public Ui_DEditGroup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_EDITGROUP_H
