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

PluginManager::PluginManager(BIN_NAMESPACE(backend)::Backend *backend) :
    _backend(backend)
{
    _plugins.clear();
}

int PluginManager::loadPlugin(const char *name)
{
    void *handle;
    DLASSERT(handle=dlopen(name, RTLD_LAZY), return -1);
    for(vector<Plugin*>::iterator it = _plugins.begin();
            it != _plugins.end(); it++)
    {
        if((*it)->handle == handle) {
            DLASSERT(dlclose(handle), NOOP);
            return -1;
        }
    }
    void *sym;
    DLASSERT(sym=dlsym(handle, "createPlugin"), return -1);
    typedef Plugin *(*PluginCreator)(PluginManager *);
    Plugin *plugin = ((PluginCreator)sym)(this);
    if(!plugin) {
        return -1;
    }
    plugin->handle = handle;
    _plugins.push_back(plugin);
    return _plugins.size()-1;
}

QAction *PluginManager::getAction(int idx, QWidget *parent)
{
    if(idx<0 || (unsigned)idx>=_plugins.size()) {
        return NULL;
    }
    return _plugins[idx]->getAction(parent);
}

void *PluginManager::findSymbol(int idx,
        const char *name)
{
    if(idx<0 || (unsigned)idx>=_plugins.size()) {
        return NULL;
    }
    void *sym;
    DLASSERT(sym=dlsym(_plugins[idx]->handle, name), return NULL);
    return sym;
}

Plugin *PluginManager::getPlugin(const char *name)
{
    for(vector<Plugin*>::iterator it = _plugins.begin();
            it != _plugins.end(); it++)
    {
        if(!strcmp((*it)->getName(), name)) {
            return *it;
        }
    }
    return NULL;
}

Plugin *PluginManager::getPlugin(int idx)
{
    if(idx<0 || (unsigned)idx>=_plugins.size()) {
        return NULL;
    }
    return _plugins[idx];
}

BIN_NAMESPACE(backend)::Backend *PluginManager::getBackend()
{
    return _backend;
}

PluginManager::~PluginManager()
{
    for(vector<Plugin*>::iterator it = _plugins.begin();
            it != _plugins.end(); it++)
    {
        void *handle = (*it)->handle;
        delete (*it);
        DLASSERT(dlclose(handle), NOOP);
    }
    _plugins.clear();
}

END_BIN_NAMESPACE
