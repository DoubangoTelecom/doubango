/********************************************************************************
** Form generated from reading UI file 'authentication.ui'
**
** Created: Wed Oct 28 02:11:54 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHENTICATION_H
#define UI_AUTHENTICATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormAuthentication
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label_PublicIdentity;
    QLineEdit *lineEdit_PublicIdentity;
    QLabel *label_PrivateIdentity;
    QLineEdit *lineEdit_PrivateIdentity;
    QLabel *label_SecretKey;
    QLineEdit *lineEdit_SecretKey;
    QLabel *label_DisplayName;
    QLineEdit *lineEdit_DisplayName;
    QLabel *label_Realm;
    QLineEdit *lineEdit_Realm;
    QLabel *label_PCSCF;
    QLineEdit *lineEdit_PCSCF;
    QSpacerItem *verticalSpacer;
    QCheckBox *checkBox_EarlyIMS;
    QSpacerItem *verticalSpacer_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_Connect;
    QPushButton *pushButton_Cancel;
    QPushButton *pushButton_Revert;

    void setupUi(QWidget *FormAuthentication)
    {
        if (FormAuthentication->objectName().isEmpty())
            FormAuthentication->setObjectName(QString::fromUtf8("FormAuthentication"));
        FormAuthentication->resize(323, 415);
        verticalLayout = new QVBoxLayout(FormAuthentication);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_PublicIdentity = new QLabel(FormAuthentication);
        label_PublicIdentity->setObjectName(QString::fromUtf8("label_PublicIdentity"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_PublicIdentity);

        lineEdit_PublicIdentity = new QLineEdit(FormAuthentication);
        lineEdit_PublicIdentity->setObjectName(QString::fromUtf8("lineEdit_PublicIdentity"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_PublicIdentity);

        label_PrivateIdentity = new QLabel(FormAuthentication);
        label_PrivateIdentity->setObjectName(QString::fromUtf8("label_PrivateIdentity"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_PrivateIdentity);

        lineEdit_PrivateIdentity = new QLineEdit(FormAuthentication);
        lineEdit_PrivateIdentity->setObjectName(QString::fromUtf8("lineEdit_PrivateIdentity"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_PrivateIdentity);

        label_SecretKey = new QLabel(FormAuthentication);
        label_SecretKey->setObjectName(QString::fromUtf8("label_SecretKey"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_SecretKey);

        lineEdit_SecretKey = new QLineEdit(FormAuthentication);
        lineEdit_SecretKey->setObjectName(QString::fromUtf8("lineEdit_SecretKey"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_SecretKey);

        label_DisplayName = new QLabel(FormAuthentication);
        label_DisplayName->setObjectName(QString::fromUtf8("label_DisplayName"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_DisplayName);

        lineEdit_DisplayName = new QLineEdit(FormAuthentication);
        lineEdit_DisplayName->setObjectName(QString::fromUtf8("lineEdit_DisplayName"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_DisplayName);

        label_Realm = new QLabel(FormAuthentication);
        label_Realm->setObjectName(QString::fromUtf8("label_Realm"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_Realm);

        lineEdit_Realm = new QLineEdit(FormAuthentication);
        lineEdit_Realm->setObjectName(QString::fromUtf8("lineEdit_Realm"));

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit_Realm);

        label_PCSCF = new QLabel(FormAuthentication);
        label_PCSCF->setObjectName(QString::fromUtf8("label_PCSCF"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_PCSCF);

        lineEdit_PCSCF = new QLineEdit(FormAuthentication);
        lineEdit_PCSCF->setObjectName(QString::fromUtf8("lineEdit_PCSCF"));

        formLayout->setWidget(5, QFormLayout::FieldRole, lineEdit_PCSCF);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(6, QFormLayout::LabelRole, verticalSpacer);

        checkBox_EarlyIMS = new QCheckBox(FormAuthentication);
        checkBox_EarlyIMS->setObjectName(QString::fromUtf8("checkBox_EarlyIMS"));

        formLayout->setWidget(7, QFormLayout::SpanningRole, checkBox_EarlyIMS);


        verticalLayout->addLayout(formLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        frame = new QFrame(FormAuthentication);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(40, 40));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_Connect = new QPushButton(frame);
        pushButton_Connect->setObjectName(QString::fromUtf8("pushButton_Connect"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_Connect->sizePolicy().hasHeightForWidth());
        pushButton_Connect->setSizePolicy(sizePolicy);
        pushButton_Connect->setAutoDefault(false);

        horizontalLayout->addWidget(pushButton_Connect);

        pushButton_Cancel = new QPushButton(frame);
        pushButton_Cancel->setObjectName(QString::fromUtf8("pushButton_Cancel"));
        sizePolicy.setHeightForWidth(pushButton_Cancel->sizePolicy().hasHeightForWidth());
        pushButton_Cancel->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton_Cancel);

        pushButton_Revert = new QPushButton(frame);
        pushButton_Revert->setObjectName(QString::fromUtf8("pushButton_Revert"));
        sizePolicy.setHeightForWidth(pushButton_Revert->sizePolicy().hasHeightForWidth());
        pushButton_Revert->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton_Revert);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(frame);


        retranslateUi(FormAuthentication);

        QMetaObject::connectSlotsByName(FormAuthentication);
    } // setupUi

    void retranslateUi(QWidget *FormAuthentication)
    {
        FormAuthentication->setWindowTitle(QApplication::translate("FormAuthentication", "Authentication", 0, QApplication::UnicodeUTF8));
        label_PublicIdentity->setText(QApplication::translate("FormAuthentication", "Public Identity*:", 0, QApplication::UnicodeUTF8));
        lineEdit_PublicIdentity->setText(QApplication::translate("FormAuthentication", "sip:doubango@example.com", 0, QApplication::UnicodeUTF8));
        label_PrivateIdentity->setText(QApplication::translate("FormAuthentication", "Private Identity*:", 0, QApplication::UnicodeUTF8));
        lineEdit_PrivateIdentity->setText(QApplication::translate("FormAuthentication", "doubango@example.com", 0, QApplication::UnicodeUTF8));
        label_SecretKey->setText(QApplication::translate("FormAuthentication", "Secret key*:", 0, QApplication::UnicodeUTF8));
        lineEdit_SecretKey->setText(QApplication::translate("FormAuthentication", "mysecret", 0, QApplication::UnicodeUTF8));
        label_DisplayName->setText(QApplication::translate("FormAuthentication", "Display Name:", 0, QApplication::UnicodeUTF8));
        lineEdit_DisplayName->setText(QApplication::translate("FormAuthentication", "Doubango Client", 0, QApplication::UnicodeUTF8));
        label_Realm->setText(QApplication::translate("FormAuthentication", "Realm*:", 0, QApplication::UnicodeUTF8));
        lineEdit_Realm->setText(QApplication::translate("FormAuthentication", "example.com", 0, QApplication::UnicodeUTF8));
        label_PCSCF->setText(QApplication::translate("FormAuthentication", "Proxy-CSCF*:", 0, QApplication::UnicodeUTF8));
        lineEdit_PCSCF->setText(QApplication::translate("FormAuthentication", "192.168.0.14:5060/UDP", 0, QApplication::UnicodeUTF8));
        checkBox_EarlyIMS->setText(QApplication::translate("FormAuthentication", "Early IMS", 0, QApplication::UnicodeUTF8));
        pushButton_Connect->setText(QApplication::translate("FormAuthentication", "Connect", 0, QApplication::UnicodeUTF8));
        pushButton_Cancel->setText(QApplication::translate("FormAuthentication", "Cancel", 0, QApplication::UnicodeUTF8));
        pushButton_Revert->setText(QApplication::translate("FormAuthentication", "Revert", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FormAuthentication: public Ui_FormAuthentication {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHENTICATION_H
