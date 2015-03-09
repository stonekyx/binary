#ifndef PLUGIN_BASIC_VIEW_H
#define PLUGIN_BASIC_VIEW_H

#include <cstdlib>
#include <map>
#include <string>

#include "common.h"
#include "frontend/PluginView.h"
#include "MainWindow.h"

BEGIN_PLUG_NAMESPACE(basic)

class View : public BIN_NAMESPACE(frontend)::PluginView {
public:
    View();
    View(std::map<std::string, std::string>);
    ~View();
    virtual void update();

    MainWindow *_mainWindow;
};

END_PLUG_NAMESPACE

#endif
