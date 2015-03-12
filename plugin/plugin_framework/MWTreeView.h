#ifndef PLUGIN_PLUGIN_FRAMEWORK_MWTREEVIEW_H
#define PLUGIN_PLUGIN_FRAMEWORK_MWTREEVIEW_H

#include <map>
#include <string>

#include "common.h"
#include "frontend/Plugin.h"
#include "backend/File.h"

#include <QMainWindow>

BEGIN_PLUG_NAMESPACE(plugin_framework)

namespace Ui {
    class MWTreeView;
}

class MWTreeView;

END_PLUG_NAMESPACE

class binary::plugin::plugin_framework::MWTreeView : public QMainWindow {
    Q_OBJECT
public:
    explicit MWTreeView(Ui::MWTreeView *ui,
            BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    virtual ~MWTreeView();
protected:
    virtual void keyPressEvent(QKeyEvent *);
public slots:
    virtual void updateInfo(binary::backend::File *);
protected:
    Ui::MWTreeView *_ui;
    BIN_NAMESPACE(frontend)::Plugin *_plugin;
};

#endif
