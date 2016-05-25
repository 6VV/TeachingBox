/********************************************************************************
** Form generated from reading UI file 'teachingbox.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEACHINGBOX_H
#define UI_TEACHINGBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TeachingBoxClass
{
public:

    void setupUi(QWidget *TeachingBoxClass)
    {
        if (TeachingBoxClass->objectName().isEmpty())
            TeachingBoxClass->setObjectName(QStringLiteral("TeachingBoxClass"));
        TeachingBoxClass->resize(600, 400);

        retranslateUi(TeachingBoxClass);

        QMetaObject::connectSlotsByName(TeachingBoxClass);
    } // setupUi

    void retranslateUi(QWidget *TeachingBoxClass)
    {
        TeachingBoxClass->setWindowTitle(QApplication::translate("TeachingBoxClass", "TeachingBox", 0));
    } // retranslateUi

};

namespace Ui {
    class TeachingBoxClass: public Ui_TeachingBoxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEACHINGBOX_H
