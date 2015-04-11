#ifndef PLUGIN_SCNDATA_UI_MAINWINDOW_H
#define PLUGIN_SCNDATA_UI_MAINWINDOW_H

#include <elf.h>

#include <QtCore/QList>
#include <QtGui/QScrollBar>
#include <QtGui/QAbstractSlider>
#include <QtGui/QGridLayout>

#include "ui_MWBase.h"
#include "ScnDataTextEdit.h"
#include "DataValue.h"

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
    ~MainWindow();

    ScnDataTextEdit *hexTextEdit;
    ScnDataTextEdit *rawTextEdit;
    ScnDataTextEdit *addrTextEdit;
    QList<ScnDataTextEdit*> textEdits;

    QGridLayout *dataAreaLayout;
    QList<DataValue*> dataValues;

    virtual bool switchMode(bool file);

    virtual void setupUi(QMainWindow *);
    void retranslateUi(QMainWindow *) { }
    void connectTE();
    void disconnectTE();
public slots:
    void setDataValue(const char *, size_t);
signals:
    void signalDataValue(const char *, size_t);
};

#endif
