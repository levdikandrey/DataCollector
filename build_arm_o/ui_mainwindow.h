/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionAddAVP;
    QAction *actionExcel;
    QAction *action_2;
    QAction *actionViolation;
    QAction *actionGroup;
    QAction *actionUser;
    QAction *actionLog;
    QAction *actionClearDB;
    QAction *actionKinopoisk;
    QAction *action;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QToolButton *toolButtonAddAVP;
    QToolButton *toolButtonExcel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelStateConnectDB;
    QToolButton *toolButtonSettingsDB;
    QToolButton *toolButtonAbout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonReload;
    QPushButton *pushButtonExit;
    QToolButton *toolButtonKinopoisk;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonViolation;
    QRadioButton *radioButtonChecked;
    QRadioButton *radioButtonAll;
    QSpacerItem *horizontalSpacer_7;
    QLineEdit *lineEditFindString;
    QToolButton *toolButton;
    QTableWidget *tableWidgetAVP;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButtonAddAVP;
    QPushButton *pushButtonDeleteAVP;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButtonPreview;
    QLabel *label_2;
    QLineEdit *lineEditNumberPage;
    QLabel *labelCountAVP;
    QPushButton *pushButtonNext;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_5;
    QComboBox *comboBoxAVS;
    QWidget *tab_3;
    QGridLayout *gridLayout_8;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBoxUser;
    QGridLayout *gridLayout_4;
    QComboBox *comboBoxUser;
    QGroupBox *groupBoxStatus;
    QGridLayout *gridLayout_5;
    QComboBox *comboBoxStatus;
    QGroupBox *groupBoxPriority;
    QGridLayout *gridLayout_9;
    QComboBox *comboBoxPriority;
    QGroupBox *groupBoxDate;
    QGridLayout *gridLayout_6;
    QDateEdit *dateEditDateEnd;
    QDateEdit *dateEditDateBegin;
    QLabel *label;
    QLabel *label_3;
    QGridLayout *gridLayout_7;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButtonReview;
    QSpacerItem *horizontalSpacer_8;
    QTableWidget *tableWidgetCurrentTasks;
    QPushButton *pushButtonNew;
    QPushButton *pushButtonEdit;
    QPushButton *pushButtonDelete;
    QSpacerItem *horizontalSpacer_4;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QTableWidget *tableWidgetMyTasks;
    QPushButton *pushButtonEditOperator;
    QPushButton *pushButtonAddAVP_2;
    QSpacerItem *horizontalSpacer_5;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1256, 796);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/grfc.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon1);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon2);
        actionAddAVP = new QAction(MainWindow);
        actionAddAVP->setObjectName(QString::fromUtf8("actionAddAVP"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/add_db.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddAVP->setIcon(icon3);
        actionExcel = new QAction(MainWindow);
        actionExcel->setObjectName(QString::fromUtf8("actionExcel"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/excel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExcel->setIcon(icon4);
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icons/configure_db.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_2->setIcon(icon5);
        actionViolation = new QAction(MainWindow);
        actionViolation->setObjectName(QString::fromUtf8("actionViolation"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/icons/category.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionViolation->setIcon(icon6);
        actionGroup = new QAction(MainWindow);
        actionGroup->setObjectName(QString::fromUtf8("actionGroup"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/icons/addusergroup.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionGroup->setIcon(icon7);
        actionUser = new QAction(MainWindow);
        actionUser->setObjectName(QString::fromUtf8("actionUser"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/icons/adduser.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionUser->setIcon(icon8);
        actionLog = new QAction(MainWindow);
        actionLog->setObjectName(QString::fromUtf8("actionLog"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/icons/edit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionLog->setIcon(icon9);
        actionClearDB = new QAction(MainWindow);
        actionClearDB->setObjectName(QString::fromUtf8("actionClearDB"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/icons/deleteBD.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClearDB->setIcon(icon10);
        actionKinopoisk = new QAction(MainWindow);
        actionKinopoisk->setObjectName(QString::fromUtf8("actionKinopoisk"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/icons/rubric.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionKinopoisk->setIcon(icon11);
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/icons/group1.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action->setIcon(icon12);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        toolButtonAddAVP = new QToolButton(centralwidget);
        toolButtonAddAVP->setObjectName(QString::fromUtf8("toolButtonAddAVP"));
        toolButtonAddAVP->setMinimumSize(QSize(32, 32));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/icons/add.ico"), QSize(), QIcon::Normal, QIcon::Off);
        toolButtonAddAVP->setIcon(icon13);
        toolButtonAddAVP->setIconSize(QSize(28, 26));

        gridLayout_2->addWidget(toolButtonAddAVP, 0, 2, 1, 1);

        toolButtonExcel = new QToolButton(centralwidget);
        toolButtonExcel->setObjectName(QString::fromUtf8("toolButtonExcel"));
        toolButtonExcel->setMinimumSize(QSize(32, 32));
        toolButtonExcel->setIcon(icon4);
        toolButtonExcel->setIconSize(QSize(24, 24));

        gridLayout_2->addWidget(toolButtonExcel, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 5, 1, 1);

        labelStateConnectDB = new QLabel(centralwidget);
        labelStateConnectDB->setObjectName(QString::fromUtf8("labelStateConnectDB"));
        labelStateConnectDB->setMinimumSize(QSize(200, 30));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(204, 208, 203, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        labelStateConnectDB->setPalette(palette);
        labelStateConnectDB->setAutoFillBackground(true);
        labelStateConnectDB->setFrameShape(QFrame::StyledPanel);
        labelStateConnectDB->setFrameShadow(QFrame::Plain);
        labelStateConnectDB->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(labelStateConnectDB, 0, 6, 1, 1);

        toolButtonSettingsDB = new QToolButton(centralwidget);
        toolButtonSettingsDB->setObjectName(QString::fromUtf8("toolButtonSettingsDB"));
        toolButtonSettingsDB->setMinimumSize(QSize(32, 32));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icons/icons/cfDB.ico"), QSize(), QIcon::Normal, QIcon::Off);
        toolButtonSettingsDB->setIcon(icon14);
        toolButtonSettingsDB->setIconSize(QSize(28, 28));

        gridLayout_2->addWidget(toolButtonSettingsDB, 0, 3, 1, 1);

        toolButtonAbout = new QToolButton(centralwidget);
        toolButtonAbout->setObjectName(QString::fromUtf8("toolButtonAbout"));
        toolButtonAbout->setMinimumSize(QSize(32, 32));
        toolButtonAbout->setIcon(icon2);
        toolButtonAbout->setIconSize(QSize(28, 28));

        gridLayout_2->addWidget(toolButtonAbout, 0, 4, 1, 1);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(693, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonReload = new QPushButton(frame);
        pushButtonReload->setObjectName(QString::fromUtf8("pushButtonReload"));
        pushButtonReload->setMinimumSize(QSize(120, 33));
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        pushButtonReload->setFont(font);
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/icons/button_reload.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonReload->setIcon(icon15);
        pushButtonReload->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonReload);

        pushButtonExit = new QPushButton(frame);
        pushButtonExit->setObjectName(QString::fromUtf8("pushButtonExit"));
        pushButtonExit->setMinimumSize(QSize(120, 33));
        pushButtonExit->setMaximumSize(QSize(16777215, 16777215));
        pushButtonExit->setFont(font);
        pushButtonExit->setIcon(icon1);
        pushButtonExit->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(pushButtonExit);


        gridLayout_2->addWidget(frame, 2, 0, 1, 7);

        toolButtonKinopoisk = new QToolButton(centralwidget);
        toolButtonKinopoisk->setObjectName(QString::fromUtf8("toolButtonKinopoisk"));
        toolButtonKinopoisk->setMinimumSize(QSize(32, 32));
        toolButtonKinopoisk->setIcon(icon11);
        toolButtonKinopoisk->setIconSize(QSize(24, 24));

        gridLayout_2->addWidget(toolButtonKinopoisk, 0, 1, 1, 1);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 0));
        groupBox->setCheckable(false);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButtonViolation = new QRadioButton(groupBox);
        radioButtonViolation->setObjectName(QString::fromUtf8("radioButtonViolation"));
        radioButtonViolation->setChecked(false);

        horizontalLayout_2->addWidget(radioButtonViolation);

        radioButtonChecked = new QRadioButton(groupBox);
        radioButtonChecked->setObjectName(QString::fromUtf8("radioButtonChecked"));

        horizontalLayout_2->addWidget(radioButtonChecked);

        radioButtonAll = new QRadioButton(groupBox);
        radioButtonAll->setObjectName(QString::fromUtf8("radioButtonAll"));
        radioButtonAll->setChecked(true);

        horizontalLayout_2->addWidget(radioButtonAll);


        horizontalLayout_3->addWidget(groupBox);

        horizontalSpacer_7 = new QSpacerItem(772, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        lineEditFindString = new QLineEdit(tab);
        lineEditFindString->setObjectName(QString::fromUtf8("lineEditFindString"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditFindString->sizePolicy().hasHeightForWidth());
        lineEditFindString->setSizePolicy(sizePolicy);
        lineEditFindString->setMinimumSize(QSize(200, 0));
        lineEditFindString->setMaximumSize(QSize(300, 16777215));

        horizontalLayout_3->addWidget(lineEditFindString);

        toolButton = new QToolButton(tab);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setMinimumSize(QSize(24, 24));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icons/icons/find.ico"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon16);

        horizontalLayout_3->addWidget(toolButton);


        gridLayout->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        tableWidgetAVP = new QTableWidget(tab);
        if (tableWidgetAVP->columnCount() < 9)
            tableWidgetAVP->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidgetAVP->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        tableWidgetAVP->setObjectName(QString::fromUtf8("tableWidgetAVP"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        tableWidgetAVP->setFont(font1);
        tableWidgetAVP->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidgetAVP->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetAVP->horizontalHeader()->setMinimumSectionSize(0);
        tableWidgetAVP->verticalHeader()->setCascadingSectionResizes(false);

        gridLayout->addWidget(tableWidgetAVP, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButtonAddAVP = new QPushButton(tab);
        pushButtonAddAVP->setObjectName(QString::fromUtf8("pushButtonAddAVP"));
        pushButtonAddAVP->setMinimumSize(QSize(120, 33));
        pushButtonAddAVP->setIcon(icon13);
        pushButtonAddAVP->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(pushButtonAddAVP);

        pushButtonDeleteAVP = new QPushButton(tab);
        pushButtonDeleteAVP->setObjectName(QString::fromUtf8("pushButtonDeleteAVP"));
        pushButtonDeleteAVP->setMinimumSize(QSize(120, 33));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/icons/icons/delete.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonDeleteAVP->setIcon(icon17);
        pushButtonDeleteAVP->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(pushButtonDeleteAVP);

        horizontalSpacer_6 = new QSpacerItem(67, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        pushButtonPreview = new QPushButton(tab);
        pushButtonPreview->setObjectName(QString::fromUtf8("pushButtonPreview"));
        pushButtonPreview->setMinimumSize(QSize(0, 33));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        pushButtonPreview->setFont(font2);
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/icons/icons/previous.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonPreview->setIcon(icon18);
        pushButtonPreview->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(pushButtonPreview);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        lineEditNumberPage = new QLineEdit(tab);
        lineEditNumberPage->setObjectName(QString::fromUtf8("lineEditNumberPage"));
        lineEditNumberPage->setMaximumSize(QSize(50, 16777215));
        lineEditNumberPage->setInputMethodHints(Qt::ImhNone);

        horizontalLayout_4->addWidget(lineEditNumberPage);

        labelCountAVP = new QLabel(tab);
        labelCountAVP->setObjectName(QString::fromUtf8("labelCountAVP"));

        horizontalLayout_4->addWidget(labelCountAVP);

        pushButtonNext = new QPushButton(tab);
        pushButtonNext->setObjectName(QString::fromUtf8("pushButtonNext"));
        pushButtonNext->setMinimumSize(QSize(0, 33));
        pushButtonNext->setFont(font2);
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/icons/icons/next.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonNext->setIcon(icon19);
        pushButtonNext->setIconSize(QSize(24, 24));

        horizontalLayout_4->addWidget(pushButtonNext);

        horizontalSpacer_3 = new QSpacerItem(70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_4->addWidget(label_5);

        comboBoxAVS = new QComboBox(tab);
        comboBoxAVS->addItem(QString());
        comboBoxAVS->setObjectName(QString::fromUtf8("comboBoxAVS"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBoxAVS->sizePolicy().hasHeightForWidth());
        comboBoxAVS->setSizePolicy(sizePolicy1);
        comboBoxAVS->setMinimumSize(QSize(300, 0));
        comboBoxAVS->setMaximumSize(QSize(3000, 16777215));

        horizontalLayout_4->addWidget(comboBoxAVS);


        gridLayout->addLayout(horizontalLayout_4, 2, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_8 = new QGridLayout(tab_3);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        groupBox_2 = new QGroupBox(tab_3);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setCheckable(false);
        horizontalLayout_5 = new QHBoxLayout(groupBox_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        groupBoxUser = new QGroupBox(groupBox_2);
        groupBoxUser->setObjectName(QString::fromUtf8("groupBoxUser"));
        groupBoxUser->setEnabled(true);
        groupBoxUser->setMinimumSize(QSize(200, 0));
        groupBoxUser->setCheckable(true);
        groupBoxUser->setChecked(false);
        gridLayout_4 = new QGridLayout(groupBoxUser);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        comboBoxUser = new QComboBox(groupBoxUser);
        comboBoxUser->setObjectName(QString::fromUtf8("comboBoxUser"));

        gridLayout_4->addWidget(comboBoxUser, 0, 0, 1, 1);


        horizontalLayout_5->addWidget(groupBoxUser);

        groupBoxStatus = new QGroupBox(groupBox_2);
        groupBoxStatus->setObjectName(QString::fromUtf8("groupBoxStatus"));
        groupBoxStatus->setEnabled(true);
        groupBoxStatus->setMinimumSize(QSize(200, 0));
        groupBoxStatus->setCheckable(true);
        groupBoxStatus->setChecked(false);
        gridLayout_5 = new QGridLayout(groupBoxStatus);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        comboBoxStatus = new QComboBox(groupBoxStatus);
        comboBoxStatus->setObjectName(QString::fromUtf8("comboBoxStatus"));

        gridLayout_5->addWidget(comboBoxStatus, 0, 0, 1, 1);


        horizontalLayout_5->addWidget(groupBoxStatus);

        groupBoxPriority = new QGroupBox(groupBox_2);
        groupBoxPriority->setObjectName(QString::fromUtf8("groupBoxPriority"));
        groupBoxPriority->setMinimumSize(QSize(200, 0));
        groupBoxPriority->setCheckable(true);
        groupBoxPriority->setChecked(false);
        gridLayout_9 = new QGridLayout(groupBoxPriority);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        comboBoxPriority = new QComboBox(groupBoxPriority);
        comboBoxPriority->setObjectName(QString::fromUtf8("comboBoxPriority"));

        gridLayout_9->addWidget(comboBoxPriority, 0, 0, 1, 1);


        horizontalLayout_5->addWidget(groupBoxPriority);

        groupBoxDate = new QGroupBox(groupBox_2);
        groupBoxDate->setObjectName(QString::fromUtf8("groupBoxDate"));
        groupBoxDate->setEnabled(true);
        groupBoxDate->setMinimumSize(QSize(220, 0));
        groupBoxDate->setCheckable(true);
        groupBoxDate->setChecked(false);
        gridLayout_6 = new QGridLayout(groupBoxDate);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        dateEditDateEnd = new QDateEdit(groupBoxDate);
        dateEditDateEnd->setObjectName(QString::fromUtf8("dateEditDateEnd"));
        dateEditDateEnd->setDateTime(QDateTime(QDate(2020, 1, 1), QTime(0, 0, 0)));

        gridLayout_6->addWidget(dateEditDateEnd, 0, 3, 1, 1);

        dateEditDateBegin = new QDateEdit(groupBoxDate);
        dateEditDateBegin->setObjectName(QString::fromUtf8("dateEditDateBegin"));
        dateEditDateBegin->setDateTime(QDateTime(QDate(2020, 1, 1), QTime(0, 0, 0)));

        gridLayout_6->addWidget(dateEditDateBegin, 0, 1, 1, 1);

        label = new QLabel(groupBoxDate);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_6->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(groupBoxDate);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_6->addWidget(label_3, 0, 2, 1, 1);


        horizontalLayout_5->addWidget(groupBoxDate);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_7->addItem(verticalSpacer, 0, 0, 1, 1);

        pushButtonReview = new QPushButton(groupBox_2);
        pushButtonReview->setObjectName(QString::fromUtf8("pushButtonReview"));
        pushButtonReview->setIcon(icon15);

        gridLayout_7->addWidget(pushButtonReview, 1, 0, 1, 1);


        horizontalLayout_5->addLayout(gridLayout_7);


        gridLayout_8->addWidget(groupBox_2, 0, 0, 1, 4);

        horizontalSpacer_8 = new QSpacerItem(261, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_8, 0, 4, 1, 1);

        tableWidgetCurrentTasks = new QTableWidget(tab_3);
        if (tableWidgetCurrentTasks->columnCount() < 10)
            tableWidgetCurrentTasks->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(3, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(4, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(5, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(6, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(7, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(8, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidgetCurrentTasks->setHorizontalHeaderItem(9, __qtablewidgetitem18);
        tableWidgetCurrentTasks->setObjectName(QString::fromUtf8("tableWidgetCurrentTasks"));
        tableWidgetCurrentTasks->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetCurrentTasks->horizontalHeader()->setMinimumSectionSize(0);
        tableWidgetCurrentTasks->horizontalHeader()->setStretchLastSection(false);

        gridLayout_8->addWidget(tableWidgetCurrentTasks, 1, 0, 1, 5);

        pushButtonNew = new QPushButton(tab_3);
        pushButtonNew->setObjectName(QString::fromUtf8("pushButtonNew"));
        pushButtonNew->setMinimumSize(QSize(120, 33));
        pushButtonNew->setFont(font1);
        pushButtonNew->setIcon(icon13);
        pushButtonNew->setIconSize(QSize(24, 24));

        gridLayout_8->addWidget(pushButtonNew, 2, 0, 1, 1);

        pushButtonEdit = new QPushButton(tab_3);
        pushButtonEdit->setObjectName(QString::fromUtf8("pushButtonEdit"));
        pushButtonEdit->setMinimumSize(QSize(120, 33));
        pushButtonEdit->setFont(font1);
        pushButtonEdit->setIcon(icon9);
        pushButtonEdit->setIconSize(QSize(24, 24));

        gridLayout_8->addWidget(pushButtonEdit, 2, 1, 1, 1);

        pushButtonDelete = new QPushButton(tab_3);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));
        pushButtonDelete->setMinimumSize(QSize(120, 33));
        pushButtonDelete->setFont(font1);
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/icons/icons/delete_png.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonDelete->setIcon(icon20);
        pushButtonDelete->setIconSize(QSize(24, 24));

        gridLayout_8->addWidget(pushButtonDelete, 2, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(944, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_4, 2, 3, 1, 2);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tableWidgetMyTasks = new QTableWidget(tab_2);
        if (tableWidgetMyTasks->columnCount() < 9)
            tableWidgetMyTasks->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(0, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(1, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(2, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(3, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(4, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(5, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(6, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(7, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        tableWidgetMyTasks->setHorizontalHeaderItem(8, __qtablewidgetitem27);
        tableWidgetMyTasks->setObjectName(QString::fromUtf8("tableWidgetMyTasks"));
        tableWidgetMyTasks->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetMyTasks->horizontalHeader()->setMinimumSectionSize(0);
        tableWidgetMyTasks->horizontalHeader()->setStretchLastSection(false);

        gridLayout_3->addWidget(tableWidgetMyTasks, 0, 0, 1, 3);

        pushButtonEditOperator = new QPushButton(tab_2);
        pushButtonEditOperator->setObjectName(QString::fromUtf8("pushButtonEditOperator"));
        pushButtonEditOperator->setMinimumSize(QSize(170, 33));
        pushButtonEditOperator->setIcon(icon9);
        pushButtonEditOperator->setIconSize(QSize(24, 24));

        gridLayout_3->addWidget(pushButtonEditOperator, 1, 0, 1, 1);

        pushButtonAddAVP_2 = new QPushButton(tab_2);
        pushButtonAddAVP_2->setObjectName(QString::fromUtf8("pushButtonAddAVP_2"));
        pushButtonAddAVP_2->setMinimumSize(QSize(120, 33));
        pushButtonAddAVP_2->setIcon(icon13);
        pushButtonAddAVP_2->setIconSize(QSize(24, 24));

        gridLayout_3->addWidget(pushButtonAddAVP_2, 1, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(900, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 1, 2, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout_2->addWidget(tabWidget, 1, 0, 1, 7);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1256, 21));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_4->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(actionExcel);
        menu->addAction(actionKinopoisk);
        menu->addSeparator();
        menu->addAction(action);
        menu->addSeparator();
        menu->addAction(actionExit);
        menu_2->addAction(action_2);
        menu_2->addSeparator();
        menu_2->addAction(actionAddAVP);
        menu_2->addAction(actionViolation);
        menu_3->addAction(actionAbout);
        menu_4->addAction(actionGroup);
        menu_4->addAction(actionUser);
        menu_4->addSeparator();
        menu_4->addAction(actionLog);
        menu_4->addAction(actionClearDB);

        retranslateUi(MainWindow);
        QObject::connect(pushButtonExit, SIGNAL(clicked()), MainWindow, SLOT(slotExit()));
        QObject::connect(pushButtonAddAVP, SIGNAL(clicked()), MainWindow, SLOT(slotAddAVP()));
        QObject::connect(pushButtonAddAVP_2, SIGNAL(clicked()), MainWindow, SLOT(slotAddAVP()));
        QObject::connect(pushButtonEditOperator, SIGNAL(clicked()), MainWindow, SLOT(slotEditMyTask()));
        QObject::connect(pushButtonEdit, SIGNAL(clicked()), MainWindow, SLOT(slotEditTask()));
        QObject::connect(pushButtonDelete, SIGNAL(clicked()), MainWindow, SLOT(slotDeleteTask()));
        QObject::connect(pushButtonNew, SIGNAL(clicked()), MainWindow, SLOT(slotAddTask()));
        QObject::connect(toolButtonAbout, SIGNAL(clicked()), MainWindow, SLOT(slotAbout()));
        QObject::connect(toolButtonSettingsDB, SIGNAL(clicked()), MainWindow, SLOT(slotSettingsDB()));
        QObject::connect(toolButtonAddAVP, SIGNAL(clicked()), MainWindow, SLOT(slotAddAVP()));
        QObject::connect(toolButtonExcel, SIGNAL(clicked()), MainWindow, SLOT(slotImportData()));
        QObject::connect(pushButtonDeleteAVP, SIGNAL(clicked()), MainWindow, SLOT(slotDeleteAVP()));
        QObject::connect(pushButtonReload, SIGNAL(clicked()), MainWindow, SLOT(slotReload()));
        QObject::connect(toolButtonKinopoisk, SIGNAL(clicked()), MainWindow, SLOT(slotImportKinopoisk()));
        QObject::connect(pushButtonNext, SIGNAL(clicked()), MainWindow, SLOT(slotNext()));
        QObject::connect(pushButtonPreview, SIGNAL(clicked()), MainWindow, SLOT(slotPrevious()));
        QObject::connect(lineEditNumberPage, SIGNAL(returnPressed()), MainWindow, SLOT(slotChangeNumberPage()));
        QObject::connect(comboBoxAVS, SIGNAL(activated(QString)), MainWindow, SLOT(slotSelectAVS(QString)));
        QObject::connect(toolButton, SIGNAL(clicked()), MainWindow, SLOT(slotFindAVP()));
        QObject::connect(pushButtonReview, SIGNAL(clicked()), MainWindow, SLOT(slotFilterApply()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\220\320\240\320\234 \320\276\320\277\320\265\321\200\320\260\321\202\320\276\321\200\320\260 \320\274\320\276\320\275\320\270\321\202\320\276\321\200\320\270\320\275\320\263\320\260 \320\220\320\222\320\237 \320\275\320\260 \320\220\320\222\320\241", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", nullptr));
#ifndef QT_NO_SHORTCUT
        actionExit->setShortcut(QApplication::translate("MainWindow", "F10", nullptr));
#endif // QT_NO_SHORTCUT
        actionAbout->setText(QApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
#ifndef QT_NO_SHORTCUT
        actionAbout->setShortcut(QApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_NO_SHORTCUT
        actionAddAVP->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\220\320\222\320\237", nullptr));
        actionExcel->setText(QApplication::translate("MainWindow", "\320\230\320\274\320\277\320\276\321\200\321\202 \320\220\320\222\320\237 \320\270\320\267 Excel", nullptr));
#ifndef QT_NO_TOOLTIP
        actionExcel->setToolTip(QApplication::translate("MainWindow", "\320\230\320\274\320\277\320\276\321\200\321\202 \320\220\320\222\320\237 \320\270\320\267 Excel", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionExcel->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        action_2->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217 \320\272 \320\221\320\224", nullptr));
        actionViolation->setText(QApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\276\321\207\320\275\320\270\320\272 \320\275\320\260\321\200\321\203\321\210\320\265\320\275\320\270\320\271", nullptr));
        actionGroup->setText(QApplication::translate("MainWindow", "\320\223\321\200\321\203\320\277\320\277\321\213", nullptr));
        actionUser->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\320\270", nullptr));
        actionLog->setText(QApplication::translate("MainWindow", "\320\237\320\276\321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 Log - \321\204\320\260\320\271\320\273", nullptr));
        actionClearDB->setText(QApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \320\221\320\224", nullptr));
        actionKinopoisk->setText(QApplication::translate("MainWindow", "\320\230\320\274\320\277\320\276\321\200\321\202 \320\220\320\222\320\237 \320\276\321\202 kinopoisk.ru", nullptr));
#ifndef QT_NO_SHORTCUT
        actionKinopoisk->setShortcut(QApplication::translate("MainWindow", "Ctrl+K", nullptr));
#endif // QT_NO_SHORTCUT
        action->setText(QApplication::translate("MainWindow", "\320\241\320\274\320\265\320\275\320\260 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
#ifndef QT_NO_TOOLTIP
        toolButtonAddAVP->setToolTip(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\220\320\222\320\237", nullptr));
#endif // QT_NO_TOOLTIP
        toolButtonAddAVP->setText(QApplication::translate("MainWindow", "...", nullptr));
#ifndef QT_NO_TOOLTIP
        toolButtonExcel->setToolTip(QApplication::translate("MainWindow", "\320\230\320\274\320\277\320\276\321\200\321\202 \320\220\320\222\320\237 \320\270\320\267 Excel", nullptr));
#endif // QT_NO_TOOLTIP
        toolButtonExcel->setText(QApplication::translate("MainWindow", "...", nullptr));
        labelStateConnectDB->setText(QApplication::translate("MainWindow", "\320\235\320\265\321\202 \321\201\320\276\320\265\320\264\320\270\320\275\320\265\320\275\320\270\321\217 \321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\276\320\274 \320\221\320\224", nullptr));
#ifndef QT_NO_TOOLTIP
        toolButtonSettingsDB->setToolTip(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217 \320\272 \320\221\320\224", nullptr));
#endif // QT_NO_TOOLTIP
        toolButtonSettingsDB->setText(QApplication::translate("MainWindow", "...", nullptr));
#ifndef QT_NO_TOOLTIP
        toolButtonAbout->setToolTip(QApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
#endif // QT_NO_TOOLTIP
        toolButtonAbout->setText(QApplication::translate("MainWindow", "...", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonReload->setToolTip(QApplication::translate("MainWindow", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonReload->setText(QApplication::translate("MainWindow", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonExit->setToolTip(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264 \320\270\320\267 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\321\217", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonExit->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", nullptr));
#ifndef QT_NO_TOOLTIP
        toolButtonKinopoisk->setToolTip(QApplication::translate("MainWindow", "\320\230\320\274\320\277\320\276\321\200\321\202 \320\220\320\222\320\237 \320\270\320\267 kinopoisk.ru", nullptr));
#endif // QT_NO_TOOLTIP
        toolButtonKinopoisk->setText(QApplication::translate("MainWindow", "...", nullptr));
#ifndef QT_NO_TOOLTIP
        tabWidget->setToolTip(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\267\320\260\320\264\320\260\321\207\321\203", nullptr));
#endif // QT_NO_TOOLTIP
        groupBox->setTitle(QApplication::translate("MainWindow", "\320\236\321\202\320\276\320\261\321\200\320\260\320\267\320\270\321\202\321\214 \320\220\320\222\320\237", nullptr));
        radioButtonViolation->setText(QApplication::translate("MainWindow", "\320\241 \320\275\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217\320\274\320\270", nullptr));
        radioButtonChecked->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\320\265\320\275\320\275\321\213\320\265", nullptr));
        radioButtonAll->setText(QApplication::translate("MainWindow", "\320\222\321\201\320\265", nullptr));
#ifndef QT_NO_TOOLTIP
        lineEditFindString->setToolTip(QApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272 \320\220\320\222\320\237", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        toolButton->setToolTip(QApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272 \320\220\320\222\320\237", nullptr));
#endif // QT_NO_TOOLTIP
        toolButton->setText(QApplication::translate("MainWindow", "...", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetAVP->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\220\320\222\320\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetAVP->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "URL", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidgetAVP->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "\320\240\321\203\320\261\321\200\320\270\320\272\320\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidgetAVP->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "\320\223\320\276\320\264 \320\262\321\213\320\277\321\203\321\201\320\272\320\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidgetAVP->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\266\320\270\321\201\321\201\320\265\321\200/\320\220\320\262\321\202\320\276\321\200", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidgetAVP->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "\320\244\320\230\320\236, \320\262\320\275\320\265\321\201 \320\262 \320\221\320\224", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidgetAVP->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "\320\224\320\260\321\202\320\260 \320\262\320\275\320\265\321\201\320\265\320\275\320\270\321\217 \320\262 \320\221\320\224", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidgetAVP->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidgetAVP->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "ID", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonAddAVP->setToolTip(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\275\320\276\320\262\321\203\321\216 \320\267\320\260\320\277\320\270\321\201\321\214 \320\276\320\261 \320\220\320\222\320\237", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonAddAVP->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\220\320\222\320\237", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonDeleteAVP->setToolTip(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\267\320\260\320\277\320\270\321\201\320\270 \320\276\320\261 \320\220\320\222\320\237", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonDeleteAVP->setText(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\220\320\222\320\237", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonPreview->setToolTip(QApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\321\213\320\264\321\203\321\211\320\270\320\265 1000 \320\267\320\260\320\277\320\270\321\201\320\265\320\271", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonPreview->setText(QApplication::translate("MainWindow", "1000", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\320\241\321\202\321\200\320\260\320\275\320\270\321\206\320\260", nullptr));
        lineEditNumberPage->setText(QApplication::translate("MainWindow", "1", nullptr));
        labelCountAVP->setText(QApplication::translate("MainWindow", "\320\270\320\267 250 (\320\262\321\201\320\265\320\263\320\276 \320\220\320\222\320\237  250000)", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonNext->setToolTip(QApplication::translate("MainWindow", "\320\241\320\273\320\265\320\264\321\203\321\216\321\211\320\270\320\265 1000 \320\267\320\260\320\277\320\270\321\201\320\265\320\271", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonNext->setText(QApplication::translate("MainWindow", "1000", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \320\220\320\222\320\241:", nullptr));
        comboBoxAVS->setItemText(0, QApplication::translate("MainWindow", "\320\222\321\201\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        comboBoxAVS->setToolTip(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \320\220\320\222\320\241", nullptr));
#endif // QT_NO_TOOLTIP
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\320\241\320\277\320\270\321\201\320\276\320\272 \320\220\320\222\320\237", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\320\244\320\270\320\273\321\214\321\202\321\200\321\213", nullptr));
        groupBoxUser->setTitle(QApplication::translate("MainWindow", "\320\237\320\276 \320\276\320\277\320\265\321\200\320\260\321\202\320\276\321\200\320\260\320\274", nullptr));
        groupBoxStatus->setTitle(QApplication::translate("MainWindow", "\320\237\320\276 \321\201\321\202\320\260\321\202\321\203\321\201\321\203", nullptr));
        groupBoxPriority->setTitle(QApplication::translate("MainWindow", "\320\237\320\276 \320\277\321\200\320\270\320\276\321\200\320\270\321\202\320\265\321\202\321\203", nullptr));
        groupBoxDate->setTitle(QApplication::translate("MainWindow", "\320\237\320\276 \320\264\320\260\321\202\320\265", nullptr));
        label->setText(QApplication::translate("MainWindow", "\320\241:", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\320\237\320\276:", nullptr));
        pushButtonReview->setText(QApplication::translate("MainWindow", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidgetCurrentTasks->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\220\320\222\320\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidgetCurrentTasks->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "URL", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidgetCurrentTasks->horizontalHeaderItem(2);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "\320\244\320\230\320\236, \320\276\320\277\320\265\321\200\320\260\321\202\320\276\321\200\320\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidgetCurrentTasks->horizontalHeaderItem(3);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "\320\224\320\260\321\202\320\260 \320\275\320\260\320\267\320\275\320\260\321\207\320\265\320\275\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidgetCurrentTasks->horizontalHeaderItem(4);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\270\320\276\321\200\320\270\321\202\320\265\321\202", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidgetCurrentTasks->horizontalHeaderItem(5);
        ___qtablewidgetitem14->setText(QApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\321\203\321\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidgetCurrentTasks->horizontalHeaderItem(6);
        ___qtablewidgetitem15->setText(QApplication::translate("MainWindow", "% \320\267\320\260\320\262\320\265\321\200\321\210.", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidgetCurrentTasks->horizontalHeaderItem(7);
        ___qtablewidgetitem16->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidgetCurrentTasks->horizontalHeaderItem(8);
        ___qtablewidgetitem17->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\270", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidgetCurrentTasks->horizontalHeaderItem(9);
        ___qtablewidgetitem18->setText(QApplication::translate("MainWindow", "ID", nullptr));
        pushButtonNew->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonEdit->setToolTip(QApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\267\320\260\320\264\320\260\321\207\321\203", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonEdit->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
#ifndef QT_NO_TOOLTIP
        pushButtonDelete->setToolTip(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\267\320\260\320\264\320\260\321\207\321\203", nullptr));
#endif // QT_NO_TOOLTIP
        pushButtonDelete->setText(QApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\320\242\320\265\320\272\321\203\321\211\320\270\320\265 \320\267\320\260\320\264\320\260\321\207\320\270", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidgetMyTasks->horizontalHeaderItem(0);
        ___qtablewidgetitem19->setText(QApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\220\320\222\320\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidgetMyTasks->horizontalHeaderItem(1);
        ___qtablewidgetitem20->setText(QApplication::translate("MainWindow", "URL", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidgetMyTasks->horizontalHeaderItem(2);
        ___qtablewidgetitem21->setText(QApplication::translate("MainWindow", "\320\224\320\260\321\202\320\260 \320\275\320\260\320\267\320\275\320\260\321\207\320\265\320\275\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tableWidgetMyTasks->horizontalHeaderItem(3);
        ___qtablewidgetitem22->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\270\320\276\321\200\320\270\321\202\320\265\321\202", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tableWidgetMyTasks->horizontalHeaderItem(4);
        ___qtablewidgetitem23->setText(QApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\321\203\321\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tableWidgetMyTasks->horizontalHeaderItem(5);
        ___qtablewidgetitem24->setText(QApplication::translate("MainWindow", "% \320\267\320\260\320\262\320\265\321\200\321\210.", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tableWidgetMyTasks->horizontalHeaderItem(6);
        ___qtablewidgetitem25->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\200\321\203\321\210\320\265\320\275\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = tableWidgetMyTasks->horizontalHeaderItem(7);
        ___qtablewidgetitem26->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\270", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = tableWidgetMyTasks->horizontalHeaderItem(8);
        ___qtablewidgetitem27->setText(QApplication::translate("MainWindow", "ID", nullptr));
        pushButtonEditOperator->setText(QApplication::translate("MainWindow", " \320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\267\320\260\320\264\320\260\321\207\321\203", nullptr));
        pushButtonAddAVP_2->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\220\320\222\320\237", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\320\234\320\276\320\270 \320\267\320\260\320\264\320\260\321\207\320\270", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        menu_2->setTitle(QApplication::translate("MainWindow", "\320\221\320\224", nullptr));
        menu_3->setTitle(QApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        menu_4->setTitle(QApplication::translate("MainWindow", "\320\220\320\264\320\274\320\270\320\275\320\270\321\201\321\202\321\200\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
