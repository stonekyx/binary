#ifndef PLUGIN_AR_MAINWINDOW_H
#define PLUGIN_AR_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "backend/File.h"
#include "common.h"
#include "MWTreeView.h"
#include "InfoModel.h"

#include <QtGui/QMainWindow>

BEGIN_PLUG_NAMESPACE(ar)

namespace Ui {
    class MainWindow;
}

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::ar::MainWindow :
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
    void changeFileToObj(size_t);
private:
    Ui::MainWindow *_ui;
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_model;
};

#endif
