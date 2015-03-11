#ifndef PLUGIN_BASIC_MAINWINDOW_H
#define PLUGIN_BASIC_MAINWINDOW_H

#include <map>
#include <string>

#include "common.h"
#include "frontend/Plugin.h"
#include "backend/File.h"

#include "InfoModel.h"

#include <QMainWindow>

BEGIN_PLUG_NAMESPACE(basic)

namespace Ui {
    class MainWindow;
}

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::basic::MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    ~MainWindow();
protected:
    virtual void keyPressEvent(QKeyEvent *);
public slots:
    void updateInfo(binary::backend::File *);
private:
    Ui::MainWindow *ui;
    BIN_NAMESPACE(frontend)::Plugin *_plugin;
    InfoModel *_infoModel;
};

#endif
