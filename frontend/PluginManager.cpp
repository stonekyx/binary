#include <cstdlib>
#include <dlfcn.h>
#include <err.h>

#include "PluginManager.h"

using namespace std;

BEGIN_BIN_NAMESPACE(frontend)

#define DLASSERT(x, failsafe) \
        do { \
            (void)dlerror(); \
            x; \
            char *errmsg = dlerror(); \
            if(errmsg) { \
                warnx(#x ": %s", errmsg); \
                failsafe; \
            } \
        } while(0)

#define NOOP

PluginManager::PluginManager()
{
    _plugins.clear();
}

int PluginManager::loadPlugin(const char *name)
{
    void *handle;
    DLASSERT(handle=dlopen(name, RTLD_LAZY), return -1);
    for(vector<void*>::iterator it = _plugins.begin();
            it != _plugins.end(); it++)
    {
        if(*it == handle) {
            DLASSERT(dlclose(handle), NOOP);
            return -1;
        }
    }
    _plugins.push_back(handle);
    return _plugins.size()-1;
}

void *PluginManager::findSymbol(int idx,
        const char *name)
{
    if(idx<0 || (unsigned)idx>=_plugins.size()) {
        return NULL;
    }
    void *sym;
    DLASSERT(sym=dlsym(_plugins[idx], name), return NULL);
    return sym;
}

PluginManager::~PluginManager()
{
    for(vector<void*>::iterator it = _plugins.begin();
            it != _plugins.end(); it++)
    {
        DLASSERT(dlclose(*it), NOOP);
    }
}

END_BIN_NAMESPACE
