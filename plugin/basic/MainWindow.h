#ifndef PLUGIN_BASIC_MAINWINDOW_H
#define PLUGIN_BASIC_MAINWINDOW_H

#include "MWTreeView.h"
#include "InfoModel.h"

#include <QMainWindow>

BEGIN_PLUG_NAMESPACE(basic)

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::basic::MainWindow :
    public PLUG_NAMESPACE(plugin_framework)::MWTreeView {
public:
    explicit MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    virtual ~MainWindow();
public:
    virtual void updateInfo(binary::backend::File *);
    using MWBase::updateInfo;
private:
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_infoModel;
};

#endif
