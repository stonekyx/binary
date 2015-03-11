#ifndef PLUGIN_BASIC_UI_MAINWINDOW_H
#define PLUGIN_BASIC_UI_MAINWINDOW_H

#include "common.h"

#include <QtGui/QPushButton>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QTreeView>

BEGIN_PLUG_NAMESPACE(basic)

class Ui_MainWindow {
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTreeView *infoTree;
    QLabel *defaultLabel;

    void setupUi(QMainWindow *MainWindow) {
        if(MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("PluginBasicMainWindow"));
        MainWindow->resize(400, 300);

        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);

        infoTree = new QTreeView(centralWidget);
        infoTree->setObjectName(QString::fromUtf8("infoTree"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(infoTree->sizePolicy().hasHeightForWidth());
        infoTree->setSizePolicy(sizePolicy);
        infoTree->hide();

        defaultLabel = new QLabel(centralWidget);
        defaultLabel->setObjectName(QString::fromUtf8("defaultLabel"));
        sizePolicy.setHeightForWidth(defaultLabel->sizePolicy().hasHeightForWidth());
        defaultLabel->setSizePolicy(sizePolicy);
        defaultLabel->show();

        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->addWidget(infoTree);
        gridLayout->addWidget(defaultLabel);
        gridLayout->setAlignment(infoTree, Qt::AlignCenter);
        gridLayout->setAlignment(defaultLabel, Qt::AlignCenter);

        retranslateUi(MainWindow);
    }
    void retranslateUi(QMainWindow *MainWindow) {
        MainWindow->setWindowTitle(QApplication::translate("PluginBasicMainWindow", "Basic Information", 0, QApplication::UnicodeUTF8));
        defaultLabel->setText(QApplication::translate("PluginBasicMainWindow", "No file opened.", 0, QApplication::UnicodeUTF8));
    }
};

namespace Ui {
    class MainWindow : public Ui_MainWindow {};
}

END_PLUG_NAMESPACE

#endif
