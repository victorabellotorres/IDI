/********************************************************************************
** Form generated from reading UI file 'Form.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyForm
{
public:
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QRadioButton *Vermell;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_3;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *MyForm)
    {
        if (MyForm->objectName().isEmpty())
            MyForm->setObjectName("MyForm");
        MyForm->resize(673, 544);
        verticalLayout_4 = new QVBoxLayout(MyForm);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        Vermell = new QRadioButton(MyForm);
        Vermell->setObjectName("Vermell");

        horizontalLayout->addWidget(Vermell);

        label = new QLabel(MyForm);
        label->setObjectName("label");
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 3, 3);"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        radioButton_2 = new QRadioButton(MyForm);
        radioButton_2->setObjectName("radioButton_2");

        horizontalLayout_2->addWidget(radioButton_2);

        label_2 = new QLabel(MyForm);
        label_2->setObjectName("label_2");
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 170, 0);"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        radioButton_3 = new QRadioButton(MyForm);
        radioButton_3->setObjectName("radioButton_3");

        horizontalLayout_3->addWidget(radioButton_3);

        label_3 = new QLabel(MyForm);
        label_3->setObjectName("label_3");
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgb(34, 14, 255);\n"
"color: rgb(255, 233, 245);"));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_3->addLayout(verticalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        pushButton = new QPushButton(MyForm);
        pushButton->setObjectName("pushButton");

        verticalLayout_2->addWidget(pushButton);


        horizontalLayout_4->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout_4->addLayout(verticalLayout_3);


        retranslateUi(MyForm);
        QObject::connect(Vermell, &QRadioButton::clicked, radioButton_2, qOverload<>(&QRadioButton::showNormal));
        QObject::connect(Vermell, &QRadioButton::clicked, radioButton_3, qOverload<>(&QRadioButton::showNormal));
        QObject::connect(radioButton_2, &QRadioButton::clicked, Vermell, qOverload<>(&QRadioButton::showNormal));
        QObject::connect(radioButton_2, &QRadioButton::clicked, radioButton_3, qOverload<>(&QRadioButton::showNormal));
        QObject::connect(radioButton_3, &QRadioButton::clicked, radioButton_2, qOverload<>(&QRadioButton::showNormal));
        QObject::connect(radioButton_3, &QRadioButton::clicked, Vermell, qOverload<>(&QRadioButton::showNormal));
        QObject::connect(pushButton, &QPushButton::released, MyForm, qOverload<>(&QWidget::close));

        QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
        MyForm->setWindowTitle(QCoreApplication::translate("MyForm", "Form", nullptr));
        Vermell->setText(QCoreApplication::translate("MyForm", "RadioButton", nullptr));
        label->setText(QCoreApplication::translate("MyForm", "VERMELL", nullptr));
        radioButton_2->setText(QCoreApplication::translate("MyForm", "Verd", nullptr));
        label_2->setText(QCoreApplication::translate("MyForm", "VERD", nullptr));
        radioButton_3->setText(QCoreApplication::translate("MyForm", "Blau", nullptr));
        label_3->setText(QCoreApplication::translate("MyForm", "BLAU", nullptr));
        pushButton->setText(QCoreApplication::translate("MyForm", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
