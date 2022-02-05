/********************************************************************************
** Form generated from reading UI file 'eeupdater.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EEUPDATER_H
#define UI_EEUPDATER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EEUpdater
{
public:
    QWidget *centralwidget;
    QTableWidget *tableWidget;
    QPushButton *updateButton;
    QProgressBar *progressBar;
    QPushButton *checkForUpdateButton;
    QPushButton *closeButton;
    QLabel *labelFinished;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *EEUpdater)
    {
        if (EEUpdater->objectName().isEmpty())
            EEUpdater->setObjectName(QString::fromUtf8("EEUpdater"));
        EEUpdater->resize(960, 704);
        centralwidget = new QWidget(EEUpdater);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 0, 941, 601));
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        updateButton = new QPushButton(centralwidget);
        updateButton->setObjectName(QString::fromUtf8("updateButton"));
        updateButton->setGeometry(QRect(870, 630, 80, 26));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(180, 630, 571, 23));
        progressBar->setValue(24);
        checkForUpdateButton = new QPushButton(centralwidget);
        checkForUpdateButton->setObjectName(QString::fromUtf8("checkForUpdateButton"));
        checkForUpdateButton->setGeometry(QRect(10, 630, 131, 26));
        closeButton = new QPushButton(centralwidget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(780, 630, 80, 26));
        labelFinished = new QLabel(centralwidget);
        labelFinished->setObjectName(QString::fromUtf8("labelFinished"));
        labelFinished->setGeometry(QRect(180, 610, 571, 18));
        EEUpdater->setCentralWidget(centralwidget);
        menubar = new QMenuBar(EEUpdater);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 960, 23));
        EEUpdater->setMenuBar(menubar);
        statusbar = new QStatusBar(EEUpdater);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        EEUpdater->setStatusBar(statusbar);

        retranslateUi(EEUpdater);

        QMetaObject::connectSlotsByName(EEUpdater);
    } // setupUi

    void retranslateUi(QMainWindow *EEUpdater)
    {
        EEUpdater->setWindowTitle(QCoreApplication::translate("EEUpdater", "Avouch Updater", nullptr));
        updateButton->setText(QCoreApplication::translate("EEUpdater", "Update", nullptr));
        checkForUpdateButton->setText(QCoreApplication::translate("EEUpdater", "Check for update", nullptr));
        closeButton->setText(QCoreApplication::translate("EEUpdater", "Close", nullptr));
        labelFinished->setText(QCoreApplication::translate("EEUpdater", "Progress:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EEUpdater: public Ui_EEUpdater {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EEUPDATER_H
