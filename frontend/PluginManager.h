#ifndef FRONTEND_PLUGIN_MANAGER_H
#define FRONTEND_PLUGIN_MANAGER_H

#include <vector>

#include "common.h"

BEGIN_BIN_NAMESPACE(frontend)

class PluginManager {
public:
    PluginManager();
    int loadPlugin(const char *);
    void *findSymbol(int, const char *);
    ~PluginManager();
private:
    std::vector<void*> _plugins;
};

END_BIN_NAMESPACE

#endif
