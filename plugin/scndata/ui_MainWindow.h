#ifndef PLUGIN_SCNDATA_UI_MAINWINDOW_H
#define PLUGIN_SCNDATA_UI_MAINWINDOW_H

#include <elf.h>

#include <QtCore/QList>
#include <QtGui/QScrollBar>
#include <QtGui/QAbstractSlider>

#include "ui_MWBase.h"
#include "ScnDataTextEdit.h"

BEGIN_PLUG_NAMESPACE(scndata)

namespace Ui {
    class MainWindow;
}

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::scndata::Ui::MainWindow :
    public binary::plugin::plugin_framework::Ui::MWBase
{
    Q_OBJECT
public:
    MainWindow() : MWBase("PluginScnDataMainWindow", "Section data") {}

    ScnDataTextEdit *hexTextEdit;
    ScnDataTextEdit *rawTextEdit;
    ScnDataTextEdit *addrTextEdit;
    QList<ScnDataTextEdit*> textEdits;

    virtual bool switchMode(bool file);

    virtual void setupUi(QMainWindow *);
    void retranslateUi(QMainWindow *) { }

    Elf64_Off parseAddr(int);
};

#endif
