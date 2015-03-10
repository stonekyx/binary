#ifndef FRONTEND_PLUGIN_MANAGER_H
#define FRONTEND_PLUGIN_MANAGER_H

#include <vector>

#include <QtGui/QAction>

#include "common.h"
#include "Plugin.h"

BEGIN_BIN_NAMESPACE(frontend)

class PluginManager {
public:
    PluginManager();
    int loadPlugin(const char *);
    QAction *getAction(int, QWidget *);
    void *findSymbol(int, const char *);
    Plugin *getPlugin(const char *);
    Plugin *getPlugin(int);
    ~PluginManager();
private:
    std::vector<Plugin*> _plugins;
};

END_BIN_NAMESPACE

#endif
