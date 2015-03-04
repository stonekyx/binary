#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QtGui/QGridLayout>

#include "common.h"

BEGIN_BIN_NAMESPACE(frontend)

class Ui_MainWindow
{
public:
    QAction *actionFileOpen;
    QAction *actionFileQuit;
    QWidget *centralWidget;
    QLabel *labelHome;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QGridLayout *centralLayout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);

        actionFileOpen = new QAction(MainWindow);
        actionFileOpen->setObjectName(QString::fromUtf8("actionFileOpen"));

        actionFileQuit = new QAction(MainWindow);
        actionFileQuit->setObjectName(QString::fromUtf8("actionFileQuit"));

        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

        labelHome = new QLabel(centralWidget);
        labelHome->setObjectName(QString::fromUtf8("labelHome"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelHome->sizePolicy().hasHeightForWidth());
        labelHome->setSizePolicy(sizePolicy);

        centralLayout = new QGridLayout(centralWidget);
        centralLayout->addWidget(labelHome);
        centralLayout->setAlignment(labelHome, Qt::AlignCenter);

        MainWindow->setCentralWidget(centralWidget);

        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));

        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionFileOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionFileQuit);

        retranslateUi(MainWindow);
        QObject::connect(actionFileQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionFileOpen, SIGNAL(triggered()), MainWindow, SLOT(openFile()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionFileOpen->setText(QApplication::translate("MainWindow", "&Open", 0, QApplication::UnicodeUTF8));
        actionFileQuit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        labelHome->setText(QApplication::translate("MainWindow", "Binary Analyzer for ELF", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

END_BIN_NAMESPACE

#endif // UI_MAINWINDOW_H
