/********************************************************************************
** Form generated from reading UI file 'd_edittask.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_EDITTASK_H
#define UI_D_EDITTASK_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_D_EditTask
{
public:
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QTextEdit *textEditComment;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *spinBoxPercent;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonExit;
    QLabel *labelNameAVP;
    QComboBox *comboBoxStatus;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QTableWidget *tableWidgetViolation;

    void setupUi(QDialog *D_EditTask)
    {
        if (D_EditTask->objectName().isEmpty())
            D_EditTask->setObjectName(QString::fromUtf8("D_EditTask"));
        D_EditTask->resize(534, 516);
        gridLayout_3 = new QGridLayout(D_EditTask);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_3 = new QLabel(D_EditTask);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 1);

        groupBox_2 = new QGroupBox(D_EditTask);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMaximumSize(QSize(16777215, 220));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        textEditComment = new QTextEdit(groupBox_2);
        textEditComment->setObjectName(QString::fromUtf8("textEditComment"));
        textEditComment->setMaximumSize(QSize(16777215, 200));

        gridLayout_2->addWidget(textEditComment, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 4, 0, 1, 2);

        label = new QLabel(D_EditTask);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(D_EditTask);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        spinBoxPercent = new QSpinBox(D_EditTask);
        spinBoxPercent->setObjectName(QString::fromUtf8("spinBoxPercent"));
        spinBoxPercent->setMaximum(100);
        spinBoxPercent->setValue(0);

        gridLayout_3->addWidget(spinBoxPercent, 2, 1, 1, 1);

        frame = new QFrame(D_EditTask);
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


        gridLayout_3->addWidget(frame, 5, 0, 1, 2);

        labelNameAVP = new QLabel(D_EditTask);
        labelNameAVP->setObjectName(QString::fromUtf8("labelNameAVP"));

        gridLayout_3->addWidget(labelNameAVP, 0, 1, 1, 1);

        comboBoxStatus = new QComboBox(D_EditTask);
        comboBoxStatus->addItem(QString());
        comboBoxStatus->setObjectName(QString::fromUtf8("comboBoxStatus"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBoxStatus->sizePolicy().hasHeightForWidth());
        comboBoxStatus->setSizePolicy(sizePolicy);

        gridLayout_3->addWidget(comboBoxStatus, 1, 1, 1, 1);

        groupBox = new QGroupBox(D_EditTask);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidgetViolation = new QTableWidget(groupBox);
        if (tableWidgetViolation->columnCount() < 2)
            tableWidgetViolation->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetViolation->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetViolation->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidgetViolation->setObjectName(QString::fromUtf8("tableWidgetViolation"));

        gridLayout->addWidget(tableWidgetViolation, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 3, 0, 1, 2);


        retranslateUi(D_EditTask);
        QObject::connect(pushButtonApply, SIGNAL(clicked()), D_EditTask, SLOT(slotApply()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), D_EditTask, SLOT(slotCancel()));
        QObject::connect(pushButtonExit, SIGNAL(clicked()), D_EditTask, SLOT(slotExit()));

        QMetaObject::connectSlotsByName(D_EditTask);
    } // setupUi

    void retranslateUi(QDialog *D_EditTask)
    {
        D_EditTask->setWindowTitle(QApplication::translate("D_EditTask", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\267\320\260\320\264\320\260\321\207\320\270", nullptr));
        label_3->setText(QApplication::translate("D_EditTask", "\320\237\321\200\320\276\321\206\320\265\320\275\321\202 \320\263\320\276\321\202\320\276\320\262\320\275\320\276\321\201\321\202\320\270:", nullptr));
        groupBox_2->setTitle(QApplication::translate("D_EditTask", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\270", nullptr));
        textEditComment->setHtml(QApplication::translate("D_EditTask", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\235\320\265\321\202 \320\275\320\260\321\200\321\203\321\210\320\265\320\275\320\270\320\271</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        label->setText(QApplication::translate("D_EditTask", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265 \320\220\320\222\320\237:", nullptr));
        label_2->setText(QApplication::translate("D_EditTask", "\320\241\321\202\320\260\321\202\321\203\321\201 \320\267\320\260\320\264\320\260\321\207\320\270:", nullptr));
        pushButtonApply->setText(QApplication::translate("D_EditTask", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("D_EditTask", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        pushButtonExit->setText(QApplication::translate("D_EditTask", "\320\222\321\213\321\205\320\276\320\264", nullptr));
        labelNameAVP->setText(QApplication::translate("D_EditTask", "\320\234\320\260\320\266\320\276\321\200", nullptr));
        comboBoxStatus->setItemText(0, QApplication::translate("D_EditTask", "\320\222 \321\200\320\260\320\261\320\276\321\202\320\265", nullptr));

        groupBox->setTitle(QApplication::translate("D_EditTask", "\320\235\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetViolation->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("D_EditTask", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetViolation->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("D_EditTask", "% \320\236\320\261\320\275\320\260\321\200\321\203\320\266\320\265\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class D_EditTask: public Ui_D_EditTask {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_EDITTASK_H
