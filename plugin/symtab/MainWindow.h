#ifndef PLUGIN_SYMTAB_MAINWINDOW_H
#define PLUGIN_SYMTAB_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "common.h"
#include "MWTreeView.h"
#include "InfoModel.h"

#include <QtGui/QMainWindow>

BEGIN_PLUG_NAMESPACE(symtab)

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::symtab::MainWindow :
    public binary::plugin::plugin_framework::MWTreeView {
    Q_OBJECT
public:
    explicit MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    ~MainWindow();
public:
    virtual void updateInfo(binary::backend::File *);
    using MWBase::updateInfo;
public slots:
    void openDisasm();
private:
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_infoModel;
    size_t _scnIndex;

    void updateArInfo(binary::backend::File *);
    void updateElfInfo(binary::backend::File *);
    void ctxMenuTreeView();
};

#endif
