#ifndef PLUGIN_PLUGIN_FRAMEWORK_MWTREEVIEW_H
#define PLUGIN_PLUGIN_FRAMEWORK_MWTREEVIEW_H

#include "MWBase.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

namespace Ui {
    class MWTreeView;
}

class MWTreeView;

END_PLUG_NAMESPACE

class binary::plugin::plugin_framework::MWTreeView :
    public binary::plugin::plugin_framework::MWBase {
public:
    explicit MWTreeView(Ui::MWTreeView *ui,
            BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    virtual ~MWTreeView();
public:
    virtual void updateInfo(binary::backend::File *);
protected:
    Ui::MWTreeView *_ui;
};

#endif
