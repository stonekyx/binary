#ifndef PLUGIN_DISASM_MAINWINDOW_H
#define PLUGIN_DISASM_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "common.h"
#include "MWTreeView.h"
#include "InfoModel.h"

#include <QtGui/QMainWindow>

#include "LoadWorker.h"

BEGIN_PLUG_NAMESPACE(disasm)

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::disasm::MainWindow :
    public PLUG_NAMESPACE(plugin_framework)::MWTreeView {
public:
    explicit MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    ~MainWindow();
public:
    virtual void updateInfo(binary::backend::File *);
private:
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_infoModel;
    size_t _scnIndex;
    LoadWorker *_loadWorker;
};

#endif
