/********************************************************************************
** Form generated from reading UI file 'deathdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEATHDIALOG_H
#define UI_DEATHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeathDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QPushButton *resumeButton;

    void setupUi(QDialog *DeathDialog)
    {
        if (DeathDialog->objectName().isEmpty())
            DeathDialog->setObjectName("DeathDialog");
        DeathDialog->resize(480, 640);
        verticalLayoutWidget = new QWidget(DeathDialog);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(-1, -1, 481, 641));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(verticalLayoutWidget);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(widget);

        resumeButton = new QPushButton(verticalLayoutWidget);
        resumeButton->setObjectName("resumeButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(104);
        sizePolicy1.setVerticalStretch(97);
        sizePolicy1.setHeightForWidth(resumeButton->sizePolicy().hasHeightForWidth());
        resumeButton->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(resumeButton);


        retranslateUi(DeathDialog);

        QMetaObject::connectSlotsByName(DeathDialog);
    } // setupUi

    void retranslateUi(QDialog *DeathDialog)
    {
        DeathDialog->setWindowTitle(QCoreApplication::translate("DeathDialog", "Dialog", nullptr));
        resumeButton->setText(QCoreApplication::translate("DeathDialog", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeathDialog: public Ui_DeathDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEATHDIALOG_H
