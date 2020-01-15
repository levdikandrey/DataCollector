/********************************************************************************
** Form generated from reading UI file 'd_addtask.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_D_ADDTASK_H
#define UI_D_ADDTASK_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_D_AddTask
{
public:
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *label_5;
    QToolButton *toolButton;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButtonPreview;
    QLabel *label_6;
    QLineEdit *lineEditNumberPage;
    QLabel *labelCountAVP;
    QPushButton *pushButtonNext;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonExit;
    QLineEdit *lineEditFindString;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QSpacerItem *verticalSpacer_2;
    QListWidget *listWidgetJobTasks;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBoxUser;
    QLabel *label_2;
    QComboBox *comboBoxPriority;
    QLabel *label_3;
    QComboBox *comboBoxStatus;
    QTableWidget *tableWidget;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *D_AddTask)
    {
        if (D_AddTask->objectName().isEmpty())
            D_AddTask->setObjectName(QString::fromUtf8("D_AddTask"));
        D_AddTask->resize(1456, 868);
        gridLayout = new QGridLayout(D_AddTask);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(D_AddTask);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(D_AddTask);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 5, 1, 1);

        toolButton = new QToolButton(D_AddTask);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setMinimumSize(QSize(24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/find.ico"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);

        gridLayout->addWidget(toolButton, 1, 3, 1, 1);

        frame = new QFrame(D_AddTask);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_6 = new QSpacerItem(192, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        pushButtonPreview = new QPushButton(frame);
        pushButtonPreview->setObjectName(QString::fromUtf8("pushButtonPreview"));
        pushButtonPreview->setMinimumSize(QSize(0, 33));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        pushButtonPreview->setFont(font);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/previous.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonPreview->setIcon(icon1);
        pushButtonPreview->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonPreview);

        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout->addWidget(label_6);

        lineEditNumberPage = new QLineEdit(frame);
        lineEditNumberPage->setObjectName(QString::fromUtf8("lineEditNumberPage"));
        lineEditNumberPage->setMaximumSize(QSize(50, 16777215));
        lineEditNumberPage->setInputMethodHints(Qt::ImhNone);

        horizontalLayout->addWidget(lineEditNumberPage);

        labelCountAVP = new QLabel(frame);
        labelCountAVP->setObjectName(QString::fromUtf8("labelCountAVP"));

        horizontalLayout->addWidget(labelCountAVP);

        pushButtonNext = new QPushButton(frame);
        pushButtonNext->setObjectName(QString::fromUtf8("pushButtonNext"));
        pushButtonNext->setMinimumSize(QSize(0, 33));
        pushButtonNext->setFont(font);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/next.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonNext->setIcon(icon2);
        pushButtonNext->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonNext);

        horizontalSpacer = new QSpacerItem(192, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonApply = new QPushButton(frame);
        pushButtonApply->setObjectName(QString::fromUtf8("pushButtonApply"));
        pushButtonApply->setMinimumSize(QSize(120, 33));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/ok.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonApply->setIcon(icon3);
        pushButtonApply->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonApply);

        pushButtonCancel = new QPushButton(frame);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setMinimumSize(QSize(120, 33));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/cancel.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonCancel->setIcon(icon4);
        pushButtonCancel->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonCancel);

        pushButtonExit = new QPushButton(frame);
        pushButtonExit->setObjectName(QString::fromUtf8("pushButtonExit"));
        pushButtonExit->setMinimumSize(QSize(120, 33));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonExit->setIcon(icon5);
        pushButtonExit->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonExit);


        gridLayout->addWidget(frame, 3, 0, 1, 6);

        lineEditFindString = new QLineEdit(D_AddTask);
        lineEditFindString->setObjectName(QString::fromUtf8("lineEditFindString"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditFindString->sizePolicy().hasHeightForWidth());
        lineEditFindString->setSizePolicy(sizePolicy);
        lineEditFindString->setMinimumSize(QSize(200, 0));
        lineEditFindString->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(lineEditFindString, 1, 2, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton_4 = new QPushButton(D_AddTask);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(0, 33));
        pushButton_4->setMaximumSize(QSize(60, 16777215));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        pushButton_4->setFont(font1);

        verticalLayout->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(D_AddTask);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setMinimumSize(QSize(0, 33));
        pushButton_5->setMaximumSize(QSize(60, 16777215));
        pushButton_5->setFont(font1);

        verticalLayout->addWidget(pushButton_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        gridLayout->addLayout(verticalLayout, 2, 4, 1, 1);

        listWidgetJobTasks = new QListWidget(D_AddTask);
        listWidgetJobTasks->setObjectName(QString::fromUtf8("listWidgetJobTasks"));
        listWidgetJobTasks->setMaximumSize(QSize(400, 16777215));
        listWidgetJobTasks->setSelectionMode(QAbstractItemView::MultiSelection);
        listWidgetJobTasks->setSelectionBehavior(QAbstractItemView::SelectItems);

        gridLayout->addWidget(listWidgetJobTasks, 2, 5, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(D_AddTask);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        comboBoxUser = new QComboBox(D_AddTask);
        comboBoxUser->addItem(QString());
        comboBoxUser->setObjectName(QString::fromUtf8("comboBoxUser"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBoxUser->sizePolicy().hasHeightForWidth());
        comboBoxUser->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(comboBoxUser);

        label_2 = new QLabel(D_AddTask);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboBoxPriority = new QComboBox(D_AddTask);
        comboBoxPriority->addItem(QString());
        comboBoxPriority->addItem(QString());
        comboBoxPriority->addItem(QString());
        comboBoxPriority->setObjectName(QString::fromUtf8("comboBoxPriority"));
        sizePolicy1.setHeightForWidth(comboBoxPriority->sizePolicy().hasHeightForWidth());
        comboBoxPriority->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(comboBoxPriority);

        label_3 = new QLabel(D_AddTask);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        comboBoxStatus = new QComboBox(D_AddTask);
        comboBoxStatus->addItem(QString());
        comboBoxStatus->addItem(QString());
        comboBoxStatus->addItem(QString());
        comboBoxStatus->addItem(QString());
        comboBoxStatus->setObjectName(QString::fromUtf8("comboBoxStatus"));
        sizePolicy1.setHeightForWidth(comboBoxStatus->sizePolicy().hasHeightForWidth());
        comboBoxStatus->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(comboBoxStatus);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 6);

        tableWidget = new QTableWidget(D_AddTask);
        if (tableWidget->columnCount() < 6)
            tableWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->horizontalHeader()->setMinimumSectionSize(0);

        gridLayout->addWidget(tableWidget, 2, 0, 1, 4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        QWidget::setTabOrder(tableWidget, pushButtonPreview);
        QWidget::setTabOrder(pushButtonPreview, lineEditNumberPage);
        QWidget::setTabOrder(lineEditNumberPage, pushButtonNext);
        QWidget::setTabOrder(pushButtonNext, comboBoxUser);
        QWidget::setTabOrder(comboBoxUser, comboBoxPriority);
        QWidget::setTabOrder(comboBoxPriority, comboBoxStatus);
        QWidget::setTabOrder(comboBoxStatus, listWidgetJobTasks);
        QWidget::setTabOrder(listWidgetJobTasks, pushButton_4);
        QWidget::setTabOrder(pushButton_4, pushButton_5);
        QWidget::setTabOrder(pushButton_5, pushButtonApply);
        QWidget::setTabOrder(pushButtonApply, pushButtonCancel);
        QWidget::setTabOrder(pushButtonCancel, pushButtonExit);

        retranslateUi(D_AddTask);
        QObject::connect(pushButtonApply, SIGNAL(clicked()), D_AddTask, SLOT(slotApply()));
        QObject::connect(pushButtonCancel, SIGNAL(clicked()), D_AddTask, SLOT(slotCancel()));
        QObject::connect(pushButtonExit, SIGNAL(clicked()), D_AddTask, SLOT(slotExit()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), D_AddTask, SLOT(slotAdd()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), D_AddTask, SLOT(slotDelete()));
        QObject::connect(pushButtonPreview, SIGNAL(clicked()), D_AddTask, SLOT(slotPrevious()));
        QObject::connect(pushButtonNext, SIGNAL(clicked()), D_AddTask, SLOT(slotNext()));
        QObject::connect(lineEditNumberPage, SIGNAL(returnPressed()), D_AddTask, SLOT(slotChangeNumberPage()));
        QObject::connect(toolButton, SIGNAL(clicked()), D_AddTask, SLOT(slotFindAVP()));

        QMetaObject::connectSlotsByName(D_AddTask);
    } // setupUi

    void retranslateUi(QDialog *D_AddTask)
    {
        D_AddTask->setWindowTitle(QApplication::translate("D_AddTask", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265  \320\275\320\276\320\262\321\213\321\205 \320\267\320\260\320\264\320\260\321\207 \320\264\320\273\321\217 \320\274\320\276\320\275\320\270\321\202\320\276\321\200\320\270\320\275\320\263\320\260 \320\220\320\222\320\237", nullptr));
        label_4->setText(QApplication::translate("D_AddTask", "\320\241\320\277\320\270\321\201\320\276\320\272 \320\220\320\222\320\237:", nullptr));
        label_5->setText(QApplication::translate("D_AddTask", "\320\241\320\277\320\270\321\201\320\276\320\272 \320\220\320\222\320\237 \320\264\320\273\321\217 \320\277\321\200\320\276\320\262\320\265\321\200\320\272\320\270:", nullptr));
#ifndef QT_NO_TOOLTIP
        toolButton->setToolTip(QApplication::translate("D_AddTask", "\320\237\320\276\320\270\321\201\320\272 \320\220\320\222\320\237", nullptr));
#endif // QT_NO_TOOLTIP
        toolButton->setText(QApplication::translate("D_AddTask", "...", nullptr));
#ifndef QT_NO_WHATSTHIS
        frame->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        pushButtonPreview->setToolTip(QApplication::translate("D_AddTask", "\320\237\321\200\320\265\320\264\321\213\320\264\321\203\321\211\320\270\320\265 1000 \320\267\320\260\320\277\320\270\321\201\320\265\320\271", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonPreview->setText(QApplication::translate("D_AddTask", "1000", nullptr));
        label_6->setText(QApplication::translate("D_AddTask", "\320\241\321\202\321\200\320\260\320\275\320\270\321\206\320\260", nullptr));
        lineEditNumberPage->setText(QApplication::translate("D_AddTask", "1", nullptr));
        labelCountAVP->setText(QApplication::translate("D_AddTask", "\320\270\320\267 250 (\320\262\321\201\320\265\320\263\320\276 \320\220\320\222\320\237  250000)", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonNext->setToolTip(QApplication::translate("D_AddTask", "\320\241\320\273\320\265\320\264\321\203\321\216\321\211\320\270\320\265 1000 \320\267\320\260\320\277\320\270\321\201\320\265\320\271", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonNext->setText(QApplication::translate("D_AddTask", "1000", nullptr));
        pushButtonApply->setText(QApplication::translate("D_AddTask", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("D_AddTask", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        pushButtonExit->setText(QApplication::translate("D_AddTask", "\320\222\321\213\321\205\320\276\320\264", nullptr));
#ifndef QT_NO_TOOLTIP
        lineEditFindString->setToolTip(QApplication::translate("D_AddTask", "\320\237\320\276\320\270\321\201\320\272 \320\220\320\222\320\237", nullptr));
#endif // QT_NO_TOOLTIP
        pushButton_4->setText(QApplication::translate("D_AddTask", ">>", nullptr));
        pushButton_5->setText(QApplication::translate("D_AddTask", "<<", nullptr));
        label->setText(QApplication::translate("D_AddTask", "\320\241\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272:", nullptr));
        comboBoxUser->setItemText(0, QApplication::translate("D_AddTask", "\320\230\320\262\320\260\320\275\320\276\320\262 \320\230.\320\230.", nullptr));

        label_2->setText(QApplication::translate("D_AddTask", "\320\237\321\200\320\270\320\276\321\200\320\270\321\202\320\265\321\202 \320\267\320\260\320\264\320\260\321\207\320\270:", nullptr));
        comboBoxPriority->setItemText(0, QApplication::translate("D_AddTask", "\320\235\320\270\320\267\320\272\320\270\320\271", nullptr));
        comboBoxPriority->setItemText(1, QApplication::translate("D_AddTask", "\320\241\321\200\320\265\320\264\320\275\320\270\320\271", nullptr));
        comboBoxPriority->setItemText(2, QApplication::translate("D_AddTask", "\320\222\321\213\321\201\320\276\320\272\320\270\320\271", nullptr));

        label_3->setText(QApplication::translate("D_AddTask", "\320\241\321\202\320\260\321\202\321\203\321\201 \320\267\320\260\320\264\320\260\321\207\320\270:", nullptr));
        comboBoxStatus->setItemText(0, QApplication::translate("D_AddTask", "\320\235\320\276\320\262\320\260\321\217", nullptr));
        comboBoxStatus->setItemText(1, QApplication::translate("D_AddTask", "\320\222 \321\200\320\260\320\261\320\276\321\202\320\265", nullptr));
        comboBoxStatus->setItemText(2, QApplication::translate("D_AddTask", "\320\222\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\260", nullptr));
        comboBoxStatus->setItemText(3, QApplication::translate("D_AddTask", "\320\227\320\260\320\272\321\200\321\213\321\202\320\260", nullptr));

        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("D_AddTask", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\321\217 \320\220\320\222\320\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("D_AddTask", "URL", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("D_AddTask", "\320\223\320\276\320\264 \320\262\321\213\320\277\321\203\321\201\320\272\320\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("D_AddTask", "\320\240\320\265\320\266\320\270\321\201\321\201\320\265\321\200/\320\220\320\262\321\202\320\276\321\200", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("D_AddTask", "\320\235\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("D_AddTask", "ID", nullptr));
    } // retranslateUi

};

namespace Ui {
    class D_AddTask: public Ui_D_AddTask {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_D_ADDTASK_H
