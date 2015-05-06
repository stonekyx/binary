/*
 * Copyright 2015 KANG Yuxuan
 *
 * ui_mainwindow.h by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <vector>

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
    QAction *actionFileClose;
    QAction *actionFileQuit;
    QAction *actionPluginLoad;
    QWidget *centralWidget;
    QLabel *labelHome;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuPlugin;
    QGridLayout *centralLayout;

    std::vector<QAction*> pluginActions;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);

        actionFileOpen = new QAction(MainWindow);
        actionFileOpen->setObjectName(QString::fromUtf8("actionFileOpen"));

        actionFileClose = new QAction(MainWindow);
        actionFileClose->setObjectName(QString::fromUtf8("actionFileClose"));

        actionFileQuit = new QAction(MainWindow);
        actionFileQuit->setObjectName(QString::fromUtf8("actionFileQuit"));

        actionPluginLoad = new QAction(MainWindow);
        actionPluginLoad->setObjectName(QString::fromUtf8("actionPluginLoad"));

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
        centralLayout->setObjectName(QString::fromUtf8("centralLayout"));
        centralLayout->addWidget(labelHome);
        centralLayout->setAlignment(labelHome, Qt::AlignCenter);

        MainWindow->setCentralWidget(centralWidget);

        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        MainWindow->setMenuBar(menuBar);

        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuBar->addAction(menuFile->menuAction());

        menuFile->addAction(actionFileOpen);
        menuFile->addAction(actionFileClose);
        menuFile->addSeparator();
        menuFile->addAction(actionFileQuit);

        menuPlugin = new QMenu(menuBar);
        menuPlugin->setObjectName(QString::fromUtf8("menuPlugin"));
        menuBar->addAction(menuPlugin->menuAction());

        menuPlugin->addAction(actionPluginLoad);

        retranslateUi(MainWindow);
        QObject::connect(actionFileQuit, SIGNAL(triggered()), MainWindow, SLOT(safeClose()));
        QObject::connect(actionFileOpen, SIGNAL(triggered()), MainWindow, SLOT(openFile()));
        QObject::connect(actionFileClose, SIGNAL(triggered()), MainWindow, SLOT(closeFile()));
        QObject::connect(actionPluginLoad, SIGNAL(triggered()), MainWindow, SLOT(loadPlugin()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionFileOpen->setText(QApplication::translate("MainWindow", "&Open", 0, QApplication::UnicodeUTF8));
        actionFileClose->setText(QApplication::translate("MainWindow", "&Close", 0, QApplication::UnicodeUTF8));
        actionFileQuit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        actionPluginLoad->setText(QApplication::translate("MainWindow", "&Load", 0, QApplication::UnicodeUTF8));
        labelHome->setText(QApplication::translate("MainWindow", "Binary Analyzer for ELF", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuPlugin->setTitle(QApplication::translate("MainWindow", "&Plugin", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

END_BIN_NAMESPACE

#endif // UI_MAINWINDOW_H
