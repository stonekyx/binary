#ifndef PLUGIN_SHDR_MAINWINDOW_H
#define PLUGIN_SHDR_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "common.h"
#include "MWTreeView.h"
#include "InfoModel.h"

#include <QtGui/QMainWindow>
#include <QtCore/QSignalMapper>

BEGIN_PLUG_NAMESPACE(shdr)

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::shdr::MainWindow :
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
    void showSection(const QString &);
private:
    void ctxMenuTreeView();
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_infoModel;
    QSignalMapper *_ctxActionMapper;
};

#endif
