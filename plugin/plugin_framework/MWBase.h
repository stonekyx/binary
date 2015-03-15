#ifndef PLUGIN_PLUGIN_FRAMEWORK_MWBASE_H
#define PLUGIN_PLUGIN_FRAMEWORK_MWBASE_H

#include <map>
#include <string>

#include "common.h"
#include "frontend/Plugin.h"
#include "backend/File.h"

#include <QMainWindow>

BEGIN_PLUG_NAMESPACE(plugin_framework)

namespace Ui {
    class MWBase;
}

class MWBase;

END_PLUG_NAMESPACE

class binary::plugin::plugin_framework::MWBase : public QMainWindow {
    Q_OBJECT
public:
    explicit MWBase(Ui::MWBase *ui,
            BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    virtual ~MWBase();
protected:
    virtual void keyPressEvent(QKeyEvent *);
public slots:
    virtual void updateInfo(binary::backend::File *);
protected:
    Ui::MWBase *_ui;
    BIN_NAMESPACE(frontend)::Plugin *_plugin;
};

#endif
