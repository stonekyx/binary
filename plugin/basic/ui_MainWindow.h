#ifndef PLUGIN_BASIC_UI_MAINWINDOW_H
#define PLUGIN_BASIC_UI_MAINWINDOW_H

#include "common.h"

#include <QtGui/QPushButton>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QApplication>

BEGIN_PLUG_NAMESPACE(basic)

class Ui_MainWindow {
public:
    QWidget *centralWidget;
    QPushButton *buttonQuit;

    void setupUi(QMainWindow *MainWindow) {
        if(MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("PluginBasicMainWindow"));
        MainWindow->resize(400, 300);

        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);

        buttonQuit = new QPushButton(centralWidget);
        buttonQuit->setObjectName(QString::fromUtf8("buttonQuit"));
        QSizePolicy buttonQuitSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        buttonQuitSizePolicy.setHorizontalStretch(0);
        buttonQuitSizePolicy.setVerticalStretch(0);
        buttonQuitSizePolicy.setHeightForWidth(buttonQuit->sizePolicy().hasHeightForWidth());
        buttonQuit->setSizePolicy(buttonQuitSizePolicy);

        retranslateUi(MainWindow);

        QObject::connect(buttonQuit, SIGNAL(clicked()), MainWindow, SLOT(close()));
    }
    void retranslateUi(QMainWindow *MainWindow) {
        MainWindow->setWindowTitle(QApplication::translate("PluginBasicMainWindow", "Basic Information", 0, QApplication::UnicodeUTF8));
        buttonQuit->setText(QApplication::translate("PluginBasicMainWindow", "Close", 0, QApplication::UnicodeUTF8));
    }
};

namespace Ui {
    class MainWindow : public Ui_MainWindow {};
}

END_PLUG_NAMESPACE

#endif
