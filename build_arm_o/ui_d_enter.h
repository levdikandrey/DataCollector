/********************************************************************************
** Form generated from reading UI file 'd_enter.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_ENTER_H
#define UI_D_ENTER_H

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

QT_BEGIN_NAMESPACE

class Ui_D_Enter
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *comboBoxUsers;
    QLabel *label_2;
    QLineEdit *lineEditPassword;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonEnter;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *D_Enter)
    {
        if (D_Enter->objectName().isEmpty())
            D_Enter->setObjectName(QString::fromUtf8("D_Enter"));
        D_Enter->resize(453, 132);
        QFont font;
        font.setPointSize(8);
        D_Enter->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/film_maker1.ico"), QSize(), QIcon::Normal, QIcon::Off);
        D_Enter->setWindowIcon(icon);
        gridLayout = new QGridLayout(D_Enter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        label = new QLabel(D_Enter);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        label->setFont(font1);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        comboBoxUsers = new QComboBox(D_Enter);
        comboBoxUsers->setObjectName(QString::fromUtf8("comboBoxUsers"));
        comboBoxUsers->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(comboBoxUsers, 0, 1, 1, 1);

        label_2 = new QLabel(D_Enter);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEditPassword = new QLineEdit(D_Enter);
        lineEditPassword->setObjectName(QString::fromUtf8("lineEditPassword"));
        lineEditPassword->setMinimumSize(QSize(0, 25));
        lineEditPassword->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEditPassword, 1, 1, 1, 1);

        frame = new QFrame(D_Enter);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(0, 40));
        frame->setMaximumSize(QSize(16777215, 50));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 1, 1, 1);
        pushButtonEnter = new QPushButton(frame);
        pushButtonEnter->setObjectName(QString::fromUtf8("pushButtonEnter"));
        pushButtonEnter->setMinimumSize(QSize(0, 30));
        pushButtonEnter->setFont(font1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/enter.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonEnter->setIcon(icon1);
        pushButtonEnter->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonEnter);

        pushButtonCancel = new QPushButton(frame);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setMinimumSize(QSize(0, 30));
        pushButtonCancel->setFont(font1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/close.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonCancel->setIcon(icon2);
        pushButtonCancel->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonCancel);


        gridLayout->addWidget(frame, 2, 0, 1, 2);


        retranslateUi(D_Enter);
        QObject::connect(pushButtonEnter, SIGNAL(clicked()), D_Enter, SLOT(slotEnter()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), D_Enter, SLOT(slotCancel()));

        QMetaObject::connectSlotsByName(D_Enter);
    } // setupUi

    void retranslateUi(QDialog *D_Enter)
    {
        D_Enter->setWindowTitle(QApplication::translate("D_Enter", "\320\222\321\205\320\276\320\264 \320\262 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\203 \320\220\320\240\320\234-\320\236", nullptr));
        label->setText(QApplication::translate("D_Enter", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214:", nullptr));
#ifndef QT_NO_TOOLTIP
        comboBoxUsers->setToolTip(QApplication::translate("D_Enter", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("D_Enter", "\320\237\320\260\321\200\320\276\320\273\321\214:", nullptr));
#ifndef QT_NO_TOOLTIP
        lineEditPassword->setToolTip(QApplication::translate("D_Enter", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        lineEditPassword->setText(QApplication::translate("D_Enter", "123", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonEnter->setToolTip(QApplication::translate("D_Enter", "\320\222\321\205\320\276\320\264 \320\262 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\203 \320\220\320\240\320\234-\320\236", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonEnter->setText(QApplication::translate("D_Enter", "\320\222\321\205\320\276\320\264", nullptr));
        pushButtonCancel->setText(QApplication::translate("D_Enter", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class D_Enter: public Ui_D_Enter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_ENTER_H
