#ifndef PLUGIN_RELOC_MAINWINDOW_H
#define PLUGIN_RELOC_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "common.h"
#include "MWTreeView.h"
#include "InfoModel.h"

#include <QtGui/QMainWindow>

BEGIN_PLUG_NAMESPACE(reloc)

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::reloc::MainWindow :
    public binary::plugin::plugin_framework::MWTreeView
{
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

    bool scanShdr(binary::backend::File *);
};

#endif
