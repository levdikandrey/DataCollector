/********************************************************************************
** Form generated from reading UI file 'd_edittaskusers.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_EDITTASKUSERS_H
#define UI_D_EDITTASKUSERS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_D_EditTaskUser
{
public:
    QGridLayout *gridLayout;
    QComboBox *comboBoxViolation;
    QComboBox *comboBoxStatus;
    QLabel *label;
    QLabel *labelNameAVP;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonExit;
    QLabel *label_7;
    QLabel *label_4;
    QLabel *label_8;
    QSpinBox *spinBoxPercent;
    QLabel *label_6;
    QLabel *label_3;
    QTextEdit *textEditComment;
    QComboBox *comboBoxUser;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *comboBoxPriority;

    void setupUi(QDialog *D_EditTaskUser)
    {
        if (D_EditTaskUser->objectName().isEmpty())
            D_EditTaskUser->setObjectName(QString::fromUtf8("D_EditTaskUser"));
        D_EditTaskUser->resize(537, 388);
        gridLayout = new QGridLayout(D_EditTaskUser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBoxViolation = new QComboBox(D_EditTaskUser);
        comboBoxViolation->addItem(QString());
        comboBoxViolation->setObjectName(QString::fromUtf8("comboBoxViolation"));

        gridLayout->addWidget(comboBoxViolation, 5, 2, 1, 1);

        comboBoxStatus = new QComboBox(D_EditTaskUser);
        comboBoxStatus->addItem(QString());
        comboBoxStatus->setObjectName(QString::fromUtf8("comboBoxStatus"));

        gridLayout->addWidget(comboBoxStatus, 3, 2, 1, 1);

        label = new QLabel(D_EditTaskUser);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        labelNameAVP = new QLabel(D_EditTaskUser);
        labelNameAVP->setObjectName(QString::fromUtf8("labelNameAVP"));

        gridLayout->addWidget(labelNameAVP, 0, 2, 1, 1);

        frame = new QFrame(D_EditTaskUser);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(613, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

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

        pushButtonExit = new QPushButton(frame);
        pushButtonExit->setObjectName(QString::fromUtf8("pushButtonExit"));
        pushButtonExit->setMinimumSize(QSize(120, 33));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonExit->setIcon(icon2);
        pushButtonExit->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonExit);


        gridLayout->addWidget(frame, 8, 0, 1, 3);

        label_7 = new QLabel(D_EditTaskUser);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 1, 0, 1, 1);

        label_4 = new QLabel(D_EditTaskUser);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 6, 0, 1, 1);

        label_8 = new QLabel(D_EditTaskUser);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 2, 0, 1, 1);

        spinBoxPercent = new QSpinBox(D_EditTaskUser);
        spinBoxPercent->setObjectName(QString::fromUtf8("spinBoxPercent"));
        spinBoxPercent->setMaximum(100);
        spinBoxPercent->setValue(0);

        gridLayout->addWidget(spinBoxPercent, 4, 2, 1, 1);

        label_6 = new QLabel(D_EditTaskUser);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        label_3 = new QLabel(D_EditTaskUser);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        textEditComment = new QTextEdit(D_EditTaskUser);
        textEditComment->setObjectName(QString::fromUtf8("textEditComment"));

        gridLayout->addWidget(textEditComment, 7, 0, 1, 3);

        comboBoxUser = new QComboBox(D_EditTaskUser);
        comboBoxUser->addItem(QString());
        comboBoxUser->setObjectName(QString::fromUtf8("comboBoxUser"));

        gridLayout->addWidget(comboBoxUser, 1, 2, 1, 1);

        label_2 = new QLabel(D_EditTaskUser);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(402, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 6, 2, 1, 1);

        comboBoxPriority = new QComboBox(D_EditTaskUser);
        comboBoxPriority->addItem(QString());
        comboBoxPriority->setObjectName(QString::fromUtf8("comboBoxPriority"));

        gridLayout->addWidget(comboBoxPriority, 2, 2, 1, 1);

        QWidget::setTabOrder(comboBoxUser, comboBoxStatus);
        QWidget::setTabOrder(comboBoxStatus, spinBoxPercent);
        QWidget::setTabOrder(spinBoxPercent, comboBoxViolation);
        QWidget::setTabOrder(comboBoxViolation, textEditComment);
        QWidget::setTabOrder(textEditComment, pushButtonApply);
        QWidget::setTabOrder(pushButtonApply, pushButtonCancel);
        QWidget::setTabOrder(pushButtonCancel, pushButtonExit);

        retranslateUi(D_EditTaskUser);
        QObject::connect(pushButtonApply, SIGNAL(clicked()), D_EditTaskUser, SLOT(slotApply()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), D_EditTaskUser, SLOT(slotCancel()));
        QObject::connect(pushButtonExit, SIGNAL(clicked()), D_EditTaskUser, SLOT(slotExit()));

        QMetaObject::connectSlotsByName(D_EditTaskUser);
    } // setupUi

    void retranslateUi(QDialog *D_EditTaskUser)
    {
        D_EditTaskUser->setWindowTitle(QApplication::translate("D_EditTaskUser", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\267\320\260\320\264\320\260\321\207\320\270", nullptr));
        comboBoxViolation->setItemText(0, QApplication::translate("D_EditTaskUser", "\320\235\320\265\321\202 \320\275\320\260\321\200\321\203\321\210\320\265\320\275\320\270\320\271", nullptr));

        comboBoxStatus->setItemText(0, QApplication::translate("D_EditTaskUser", "\320\222 \321\200\320\260\320\261\320\276\321\202\320\265", nullptr));

        label->setText(QApplication::translate("D_EditTaskUser", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265 \320\220\320\222\320\237:", nullptr));
        labelNameAVP->setText(QApplication::translate("D_EditTaskUser", "\320\234\320\260\320\266\320\276\321\200", nullptr));
        pushButtonApply->setText(QApplication::translate("D_EditTaskUser", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("D_EditTaskUser", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        pushButtonExit->setText(QApplication::translate("D_EditTaskUser", "\320\222\321\213\321\205\320\276\320\264", nullptr));
        label_7->setText(QApplication::translate("D_EditTaskUser", "\320\244\320\230\320\236, \320\276\320\277\320\265\321\200\320\260\321\202\320\276\321\200\320\260:", nullptr));
        label_4->setText(QApplication::translate("D_EditTaskUser", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\321\200\320\260\321\200\320\270\320\271:", nullptr));
        label_8->setText(QApplication::translate("D_EditTaskUser", "\320\237\321\200\320\270\320\276\321\200\320\270\321\202\320\265\321\202:", nullptr));
        label_6->setText(QApplication::translate("D_EditTaskUser", "\320\235\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217:", nullptr));
        label_3->setText(QApplication::translate("D_EditTaskUser", "\320\237\321\200\320\276\321\206\320\265\320\275\321\202 \320\263\320\276\321\202\320\276\320\262\320\275\320\276\321\201\321\202\320\270:", nullptr));
        textEditComment->setHtml(QApplication::translate("D_EditTaskUser", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\235\320\265\321\202 \320\275\320\260\321\200\321\203\321\210\320\265\320\275\320\270\320\271</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        comboBoxUser->setItemText(0, QApplication::translate("D_EditTaskUser", "\320\237\320\265\321\202\321\200\320\276\320\262 \320\241.\320\241.", nullptr));

        label_2->setText(QApplication::translate("D_EditTaskUser", "\320\241\321\202\320\260\321\202\321\203\321\201 \320\267\320\260\320\264\320\260\321\207\320\270:", nullptr));
        comboBoxPriority->setItemText(0, QApplication::translate("D_EditTaskUser", "\320\235\320\270\320\267\320\272\320\270\320\271", nullptr));

    } // retranslateUi

};

namespace Ui {
    class D_EditTaskUser: public Ui_D_EditTaskUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_EDITTASKUSERS_H
