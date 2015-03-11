#ifndef FRONTEND_PLUGIN_MANAGER_H
#define FRONTEND_PLUGIN_MANAGER_H

#include <vector>

#include <QtGui/QAction>

#include "common.h"
#include "Plugin.h"
#include "backend/Backend.h"

BEGIN_BIN_NAMESPACE(frontend)

class PluginManager {
public:
    PluginManager(BIN_NAMESPACE(backend)::Backend *);
    int loadPlugin(const char *);
    QAction *getAction(int, QWidget *);
    void *findSymbol(int, const char *);
    Plugin *getPlugin(const char *);
    Plugin *getPlugin(int);
    BIN_NAMESPACE(backend)::Backend *getBackend();
    ~PluginManager();
private:
    std::vector<Plugin*> _plugins;
    BIN_NAMESPACE(backend)::Backend *_backend;
};

END_BIN_NAMESPACE

#endif
