/********************************************************************************
** Form generated from reading UI file 'doubs.ui'
**
** Created: Tue Oct 27 01:18:07 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOUBS_H
#define UI_DOUBS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_doubsClass
{
public:
    QAction *actionOnline;
    QAction *actionSign_Out;
    QAction *actionExit;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionAdd_Contact;
    QAction *actionEdit_Contact;
    QAction *actionDelete_Contact;
    QAction *actionAdd_Group;
    QAction *actionEdit_Group;
    QAction *actionDelete_Group;
    QAction *actionAdd_Sphere;
    QAction *actionEdit_Sphere;
    QAction *actionDelete_Sphere;
    QAction *actionGetting_started;
    QAction *actionRelease_Note;
    QAction *actionForum;
    QAction *actionCompliance;
    QAction *actionBusy;
    QAction *actionBe_right_back;
    QAction *actionAway;
    QAction *actionIn_call;
    QAction *actionOut_to_lunch;
    QAction *actionAppear_off_line;
    QAction *actionChange_your_contact_card;
    QAction *actionRegistration_info;
    QAction *actionTbarSignIn;
    QAction *actionTbarSettings;
    QAction *actionTbarAddressBook;
    QAction *actionTbarSignOut;
    QAction *actionOpen_Log_File;
    QAction *actionDiagnostics;
    QAction *actionAudio_Video_devices;
    QWidget *centralWidget;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAddress_Book;
    QMenu *menu;
    QMenu *menuTools;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *doubsClass)
    {
        if (doubsClass->objectName().isEmpty())
            doubsClass->setObjectName(QString::fromUtf8("doubsClass"));
        doubsClass->setWindowModality(Qt::ApplicationModal);
        doubsClass->resize(335, 477);
        actionOnline = new QAction(doubsClass);
        actionOnline->setObjectName(QString::fromUtf8("actionOnline"));
        actionSign_Out = new QAction(doubsClass);
        actionSign_Out->setObjectName(QString::fromUtf8("actionSign_Out"));
        actionExit = new QAction(doubsClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        actionExit->setFont(font);
        actionHelp = new QAction(doubsClass);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionAbout = new QAction(doubsClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAdd_Contact = new QAction(doubsClass);
        actionAdd_Contact->setObjectName(QString::fromUtf8("actionAdd_Contact"));
        actionEdit_Contact = new QAction(doubsClass);
        actionEdit_Contact->setObjectName(QString::fromUtf8("actionEdit_Contact"));
        actionDelete_Contact = new QAction(doubsClass);
        actionDelete_Contact->setObjectName(QString::fromUtf8("actionDelete_Contact"));
        actionAdd_Group = new QAction(doubsClass);
        actionAdd_Group->setObjectName(QString::fromUtf8("actionAdd_Group"));
        actionEdit_Group = new QAction(doubsClass);
        actionEdit_Group->setObjectName(QString::fromUtf8("actionEdit_Group"));
        actionDelete_Group = new QAction(doubsClass);
        actionDelete_Group->setObjectName(QString::fromUtf8("actionDelete_Group"));
        actionAdd_Sphere = new QAction(doubsClass);
        actionAdd_Sphere->setObjectName(QString::fromUtf8("actionAdd_Sphere"));
        actionEdit_Sphere = new QAction(doubsClass);
        actionEdit_Sphere->setObjectName(QString::fromUtf8("actionEdit_Sphere"));
        actionDelete_Sphere = new QAction(doubsClass);
        actionDelete_Sphere->setObjectName(QString::fromUtf8("actionDelete_Sphere"));
        actionGetting_started = new QAction(doubsClass);
        actionGetting_started->setObjectName(QString::fromUtf8("actionGetting_started"));
        actionRelease_Note = new QAction(doubsClass);
        actionRelease_Note->setObjectName(QString::fromUtf8("actionRelease_Note"));
        actionForum = new QAction(doubsClass);
        actionForum->setObjectName(QString::fromUtf8("actionForum"));
        actionCompliance = new QAction(doubsClass);
        actionCompliance->setObjectName(QString::fromUtf8("actionCompliance"));
        actionBusy = new QAction(doubsClass);
        actionBusy->setObjectName(QString::fromUtf8("actionBusy"));
        actionBe_right_back = new QAction(doubsClass);
        actionBe_right_back->setObjectName(QString::fromUtf8("actionBe_right_back"));
        actionAway = new QAction(doubsClass);
        actionAway->setObjectName(QString::fromUtf8("actionAway"));
        actionIn_call = new QAction(doubsClass);
        actionIn_call->setObjectName(QString::fromUtf8("actionIn_call"));
        actionOut_to_lunch = new QAction(doubsClass);
        actionOut_to_lunch->setObjectName(QString::fromUtf8("actionOut_to_lunch"));
        actionAppear_off_line = new QAction(doubsClass);
        actionAppear_off_line->setObjectName(QString::fromUtf8("actionAppear_off_line"));
        actionChange_your_contact_card = new QAction(doubsClass);
        actionChange_your_contact_card->setObjectName(QString::fromUtf8("actionChange_your_contact_card"));
        actionRegistration_info = new QAction(doubsClass);
        actionRegistration_info->setObjectName(QString::fromUtf8("actionRegistration_info"));
        actionTbarSignIn = new QAction(doubsClass);
        actionTbarSignIn->setObjectName(QString::fromUtf8("actionTbarSignIn"));
        actionTbarSettings = new QAction(doubsClass);
        actionTbarSettings->setObjectName(QString::fromUtf8("actionTbarSettings"));
        actionTbarAddressBook = new QAction(doubsClass);
        actionTbarAddressBook->setObjectName(QString::fromUtf8("actionTbarAddressBook"));
        actionTbarSignOut = new QAction(doubsClass);
        actionTbarSignOut->setObjectName(QString::fromUtf8("actionTbarSignOut"));
        actionOpen_Log_File = new QAction(doubsClass);
        actionOpen_Log_File->setObjectName(QString::fromUtf8("actionOpen_Log_File"));
        actionDiagnostics = new QAction(doubsClass);
        actionDiagnostics->setObjectName(QString::fromUtf8("actionDiagnostics"));
        actionAudio_Video_devices = new QAction(doubsClass);
        actionAudio_Video_devices->setObjectName(QString::fromUtf8("actionAudio_Video_devices"));
        centralWidget = new QWidget(doubsClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setGeometry(QRect(9, 65, 320, 351));
        mdiArea->setTabShape(QTabWidget::Rounded);
        doubsClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(doubsClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 335, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuAddress_Book = new QMenu(menuBar);
        menuAddress_Book->setObjectName(QString::fromUtf8("menuAddress_Book"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        doubsClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(doubsClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        doubsClass->setStatusBar(statusBar);
        toolBar = new QToolBar(doubsClass);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMinimumSize(QSize(33, 33));
        doubsClass->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAddress_Book->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menu->menuAction());
        menuFile->addAction(actionOnline);
        menuFile->addAction(actionBusy);
        menuFile->addAction(actionBe_right_back);
        menuFile->addAction(actionAway);
        menuFile->addAction(actionIn_call);
        menuFile->addAction(actionOut_to_lunch);
        menuFile->addAction(actionAppear_off_line);
        menuFile->addAction(actionSign_Out);
        menuFile->addSeparator();
        menuFile->addAction(actionRegistration_info);
        menuFile->addAction(actionChange_your_contact_card);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuAddress_Book->addAction(actionAdd_Contact);
        menuAddress_Book->addAction(actionEdit_Contact);
        menuAddress_Book->addAction(actionDelete_Contact);
        menuAddress_Book->addSeparator();
        menuAddress_Book->addAction(actionAdd_Group);
        menuAddress_Book->addAction(actionEdit_Group);
        menuAddress_Book->addAction(actionDelete_Group);
        menuAddress_Book->addSeparator();
        menuAddress_Book->addAction(actionAdd_Sphere);
        menuAddress_Book->addAction(actionEdit_Sphere);
        menuAddress_Book->addAction(actionDelete_Sphere);
        menu->addAction(actionForum);
        menu->addAction(actionGetting_started);
        menu->addSeparator();
        menu->addAction(actionRelease_Note);
        menu->addAction(actionCompliance);
        menu->addSeparator();
        menu->addAction(actionAbout);
        menu->addSeparator();
        menuTools->addAction(actionOpen_Log_File);
        menuTools->addAction(actionDiagnostics);
        menuTools->addSeparator();
        menuTools->addAction(actionAudio_Video_devices);
        toolBar->addAction(actionTbarSignIn);
        toolBar->addAction(actionTbarSignOut);
        toolBar->addAction(actionTbarAddressBook);
        toolBar->addAction(actionTbarSettings);

        retranslateUi(doubsClass);

        QMetaObject::connectSlotsByName(doubsClass);
    } // setupUi

    void retranslateUi(QMainWindow *doubsClass)
    {
        doubsClass->setWindowTitle(QApplication::translate("doubsClass", "doubs", 0, QApplication::UnicodeUTF8));
        actionOnline->setText(QApplication::translate("doubsClass", "Online", 0, QApplication::UnicodeUTF8));
        actionOnline->setShortcut(QApplication::translate("doubsClass", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSign_Out->setText(QApplication::translate("doubsClass", "Sign Out", 0, QApplication::UnicodeUTF8));
        actionSign_Out->setShortcut(QApplication::translate("doubsClass", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("doubsClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("doubsClass", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("doubsClass", "Help", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("doubsClass", "About...", 0, QApplication::UnicodeUTF8));
        actionAdd_Contact->setText(QApplication::translate("doubsClass", "Add Contact", 0, QApplication::UnicodeUTF8));
        actionEdit_Contact->setText(QApplication::translate("doubsClass", "Edit Contact", 0, QApplication::UnicodeUTF8));
        actionDelete_Contact->setText(QApplication::translate("doubsClass", "Delete Contact", 0, QApplication::UnicodeUTF8));
        actionAdd_Group->setText(QApplication::translate("doubsClass", "Add Group", 0, QApplication::UnicodeUTF8));
        actionEdit_Group->setText(QApplication::translate("doubsClass", "Edit Group", 0, QApplication::UnicodeUTF8));
        actionDelete_Group->setText(QApplication::translate("doubsClass", "Delete Group", 0, QApplication::UnicodeUTF8));
        actionAdd_Sphere->setText(QApplication::translate("doubsClass", "Add Sphere", 0, QApplication::UnicodeUTF8));
        actionEdit_Sphere->setText(QApplication::translate("doubsClass", "Edit Sphere", 0, QApplication::UnicodeUTF8));
        actionDelete_Sphere->setText(QApplication::translate("doubsClass", "Delete Sphere", 0, QApplication::UnicodeUTF8));
        actionGetting_started->setText(QApplication::translate("doubsClass", "Getting started", 0, QApplication::UnicodeUTF8));
        actionRelease_Note->setText(QApplication::translate("doubsClass", "Release Note", 0, QApplication::UnicodeUTF8));
        actionForum->setText(QApplication::translate("doubsClass", "Forum", 0, QApplication::UnicodeUTF8));
        actionCompliance->setText(QApplication::translate("doubsClass", "Compliance", 0, QApplication::UnicodeUTF8));
        actionBusy->setText(QApplication::translate("doubsClass", "Busy", 0, QApplication::UnicodeUTF8));
        actionBusy->setShortcut(QApplication::translate("doubsClass", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        actionBe_right_back->setText(QApplication::translate("doubsClass", "Be right back", 0, QApplication::UnicodeUTF8));
        actionBe_right_back->setShortcut(QApplication::translate("doubsClass", "Ctrl+K", 0, QApplication::UnicodeUTF8));
        actionAway->setText(QApplication::translate("doubsClass", "Away", 0, QApplication::UnicodeUTF8));
        actionAway->setShortcut(QApplication::translate("doubsClass", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionIn_call->setText(QApplication::translate("doubsClass", "In call", 0, QApplication::UnicodeUTF8));
        actionIn_call->setShortcut(QApplication::translate("doubsClass", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        actionOut_to_lunch->setText(QApplication::translate("doubsClass", "Out to lunch", 0, QApplication::UnicodeUTF8));
        actionAppear_off_line->setText(QApplication::translate("doubsClass", "Appear off line", 0, QApplication::UnicodeUTF8));
        actionChange_your_contact_card->setText(QApplication::translate("doubsClass", "Change your contact card", 0, QApplication::UnicodeUTF8));
        actionRegistration_info->setText(QApplication::translate("doubsClass", "Registration info", 0, QApplication::UnicodeUTF8));
        actionTbarSignIn->setText(QApplication::translate("doubsClass", "SignIn", 0, QApplication::UnicodeUTF8));
        actionTbarSettings->setText(QApplication::translate("doubsClass", "Settings", 0, QApplication::UnicodeUTF8));
        actionTbarAddressBook->setText(QApplication::translate("doubsClass", "AddressBook", 0, QApplication::UnicodeUTF8));
        actionTbarSignOut->setText(QApplication::translate("doubsClass", "SignOut", 0, QApplication::UnicodeUTF8));
        actionOpen_Log_File->setText(QApplication::translate("doubsClass", "Open Log File", 0, QApplication::UnicodeUTF8));
        actionDiagnostics->setText(QApplication::translate("doubsClass", "Diagnostics", 0, QApplication::UnicodeUTF8));
        actionAudio_Video_devices->setText(QApplication::translate("doubsClass", "Audio/Video devices", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("doubsClass", "&File", 0, QApplication::UnicodeUTF8));
        menuAddress_Book->setTitle(QApplication::translate("doubsClass", "&Address Book", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("doubsClass", "Help", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("doubsClass", "Tools", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("doubsClass", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class doubsClass: public Ui_doubsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOUBS_H
