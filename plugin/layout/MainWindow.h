#ifndef PLUGIN_LAYOUT_MAINWINDOW_H
#define PLUGIN_LAYOUT_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "backend/File.h"
#include "common.h"
#include "MWBase.h"

#include <QtGui/QMainWindow>

BEGIN_PLUG_NAMESPACE(layout)

namespace Ui {
    class MainWindow;
}

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::layout::MainWindow :
    public binary::plugin::plugin_framework::MWBase {
    Q_OBJECT
public:
    explicit MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    ~MainWindow();
public:
    virtual void updateInfo(binary::backend::File *);
public slots:
    void openScn(size_t scnIdx);
private:
    Ui::MainWindow *_ui;
};

#endif
