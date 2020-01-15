/********************************************************************************
** Form generated from reading UI file 'd_addavp.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_ADDAVP_H
#define UI_D_ADDAVP_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_D_AddAVP
{
public:
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLineEdit *lineEditAge;
    QLabel *label_10;
    QDateEdit *dateEditYearOfRelease;
    QLabel *label;
    QLineEdit *lineEditURL_AVS;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonExit;
    QLineEdit *lineEditFilmMaker;
    QLabel *label_3;
    QLabel *label_7;
    QLineEdit *lineEditRubric;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *lineEditURL_AVP;
    QSpinBox *spinBoxDuration;
    QLineEdit *lineEditNameOriginal;
    QLabel *label_2;
    QLineEdit *lineEditName;
    QLineEdit *lineEditFormAVP;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label_11;
    QLineEdit *lineEditNameAVS;

    void setupUi(QDialog *D_AddAVP)
    {
        if (D_AddAVP->objectName().isEmpty())
            D_AddAVP->setObjectName(QString::fromUtf8("D_AddAVP"));
        D_AddAVP->resize(610, 442);
        gridLayout = new QGridLayout(D_AddAVP);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(9, 5, -1, 3);
        label_5 = new QLabel(D_AddAVP);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 7, 0, 1, 1);

        lineEditAge = new QLineEdit(D_AddAVP);
        lineEditAge->setObjectName(QString::fromUtf8("lineEditAge"));

        gridLayout->addWidget(lineEditAge, 9, 2, 1, 1);

        label_10 = new QLabel(D_AddAVP);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 11, 0, 1, 1);

        dateEditYearOfRelease = new QDateEdit(D_AddAVP);
        dateEditYearOfRelease->setObjectName(QString::fromUtf8("dateEditYearOfRelease"));

        gridLayout->addWidget(dateEditYearOfRelease, 10, 2, 1, 1);

        label = new QLabel(D_AddAVP);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        lineEditURL_AVS = new QLineEdit(D_AddAVP);
        lineEditURL_AVS->setObjectName(QString::fromUtf8("lineEditURL_AVS"));

        gridLayout->addWidget(lineEditURL_AVS, 4, 2, 1, 1);

        frame = new QFrame(D_AddAVP);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMaximumSize(QSize(16777215, 55));
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


        gridLayout->addWidget(frame, 12, 0, 1, 3);

        lineEditFilmMaker = new QLineEdit(D_AddAVP);
        lineEditFilmMaker->setObjectName(QString::fromUtf8("lineEditFilmMaker"));

        gridLayout->addWidget(lineEditFilmMaker, 8, 2, 1, 1);

        label_3 = new QLabel(D_AddAVP);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        label_7 = new QLabel(D_AddAVP);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 8, 0, 1, 1);

        lineEditRubric = new QLineEdit(D_AddAVP);
        lineEditRubric->setObjectName(QString::fromUtf8("lineEditRubric"));

        gridLayout->addWidget(lineEditRubric, 7, 2, 1, 1);

        label_8 = new QLabel(D_AddAVP);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 9, 0, 1, 1);

        label_9 = new QLabel(D_AddAVP);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 10, 0, 1, 1);

        lineEditURL_AVP = new QLineEdit(D_AddAVP);
        lineEditURL_AVP->setObjectName(QString::fromUtf8("lineEditURL_AVP"));

        gridLayout->addWidget(lineEditURL_AVP, 5, 2, 1, 1);

        spinBoxDuration = new QSpinBox(D_AddAVP);
        spinBoxDuration->setObjectName(QString::fromUtf8("spinBoxDuration"));
        spinBoxDuration->setMaximum(10000);

        gridLayout->addWidget(spinBoxDuration, 11, 2, 1, 1);

        lineEditNameOriginal = new QLineEdit(D_AddAVP);
        lineEditNameOriginal->setObjectName(QString::fromUtf8("lineEditNameOriginal"));

        gridLayout->addWidget(lineEditNameOriginal, 2, 2, 1, 1);

        label_2 = new QLabel(D_AddAVP);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        lineEditName = new QLineEdit(D_AddAVP);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gridLayout->addWidget(lineEditName, 1, 2, 1, 1);

        lineEditFormAVP = new QLineEdit(D_AddAVP);
        lineEditFormAVP->setObjectName(QString::fromUtf8("lineEditFormAVP"));

        gridLayout->addWidget(lineEditFormAVP, 6, 2, 1, 1);

        label_6 = new QLabel(D_AddAVP);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        label_4 = new QLabel(D_AddAVP);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 6, 0, 1, 1);

        label_11 = new QLabel(D_AddAVP);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 0, 0, 1, 1);

        lineEditNameAVS = new QLineEdit(D_AddAVP);
        lineEditNameAVS->setObjectName(QString::fromUtf8("lineEditNameAVS"));

        gridLayout->addWidget(lineEditNameAVS, 0, 2, 1, 1);

        QWidget::setTabOrder(lineEditNameAVS, lineEditName);
        QWidget::setTabOrder(lineEditName, lineEditNameOriginal);
        QWidget::setTabOrder(lineEditNameOriginal, lineEditURL_AVS);
        QWidget::setTabOrder(lineEditURL_AVS, lineEditURL_AVP);
        QWidget::setTabOrder(lineEditURL_AVP, lineEditFormAVP);
        QWidget::setTabOrder(lineEditFormAVP, lineEditRubric);
        QWidget::setTabOrder(lineEditRubric, lineEditFilmMaker);
        QWidget::setTabOrder(lineEditFilmMaker, lineEditAge);
        QWidget::setTabOrder(lineEditAge, dateEditYearOfRelease);
        QWidget::setTabOrder(dateEditYearOfRelease, spinBoxDuration);
        QWidget::setTabOrder(spinBoxDuration, pushButtonApply);
        QWidget::setTabOrder(pushButtonApply, pushButtonCancel);
        QWidget::setTabOrder(pushButtonCancel, pushButtonExit);

        retranslateUi(D_AddAVP);
        QObject::connect(pushButtonApply, SIGNAL(clicked()), D_AddAVP, SLOT(slotApply()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), D_AddAVP, SLOT(slotCancel()));
        QObject::connect(pushButtonExit, SIGNAL(clicked()), D_AddAVP, SLOT(slotExit()));

        QMetaObject::connectSlotsByName(D_AddAVP);
    } // setupUi

    void retranslateUi(QDialog *D_AddAVP)
    {
        D_AddAVP->setWindowTitle(QApplication::translate("D_AddAVP", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\220\320\222\320\237", nullptr));
        label_5->setText(QApplication::translate("D_AddAVP", "\320\240\321\203\320\261\321\200\320\270\320\272\320\260: *", nullptr));
        label_10->setText(QApplication::translate("D_AddAVP", "\320\245\321\200\320\276\320\275\320\276\320\274\320\265\321\202\321\200\320\260\320\266, \320\274\320\270\320\275:", nullptr));
        label->setText(QApplication::translate("D_AddAVP", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265 \320\220\320\222\320\237 \320\275\320\260 \321\200\321\203\321\201\321\201\320\272\320\276\320\274: *", nullptr));
        pushButtonApply->setText(QApplication::translate("D_AddAVP", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("D_AddAVP", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        pushButtonExit->setText(QApplication::translate("D_AddAVP", "\320\222\321\213\321\205\320\276\320\264", nullptr));
        label_3->setText(QApplication::translate("D_AddAVP", "URL \320\220\320\222\320\241: *", nullptr));
        label_7->setText(QApplication::translate("D_AddAVP", "\320\240\320\265\320\266\320\270\321\201\321\201\320\265\321\200/\320\220\320\262\321\202\320\276\321\200: *", nullptr));
        label_8->setText(QApplication::translate("D_AddAVP", "\320\222\320\276\320\267\321\200\320\260\321\201\321\202:", nullptr));
        label_9->setText(QApplication::translate("D_AddAVP", "\320\223\320\276\320\264 \320\262\321\213\320\277\321\203\321\201\320\272\320\260: *", nullptr));
        label_2->setText(QApplication::translate("D_AddAVP", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265 \320\220\320\222\320\237 \320\275\320\260 \321\217\320\267\321\213\320\272\320\265 \320\276\321\200\320\270\320\263\320\270\320\275\320\260\320\273\320\260:", nullptr));
        label_6->setText(QApplication::translate("D_AddAVP", "URL \320\220\320\222\320\237: *", nullptr));
        label_4->setText(QApplication::translate("D_AddAVP", "\320\244\320\276\321\200\320\274\320\260 \320\220\320\222\320\237: *", nullptr));
        label_11->setText(QApplication::translate("D_AddAVP", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265 \320\220\320\222\320\241: *", nullptr));
    } // retranslateUi

};

namespace Ui {
    class D_AddAVP: public Ui_D_AddAVP {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_ADDAVP_H
