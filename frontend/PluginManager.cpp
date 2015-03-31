#include <cstdlib>
#include <dlfcn.h>
#include <err.h>

#include "Plugin.h"
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
    _backend(backend), _arrangedDelete(false)
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

void PluginManager::arrangeDelete()
{
    _arrangedDelete = true;
    for(vector<Plugin*>::iterator it = _plugins.begin();
            it != _plugins.end(); it++)
    {
        delete (*it);
    }
    _plugins.clear();
    if(_libDep.empty()) {
        deleteLater();
    }
}

void PluginManager::registerLibDep(const QObject *o, void *handle)
{
    if(!o || !handle) {
        return;
    }
    _libDep[o] = handle;
    if(_depCnt.find(handle) == _depCnt.end()) {
        _depCnt[handle] = 0;
    }
    _depCnt[handle]++;
    QObject::connect(o, SIGNAL(destroyed()),
            this, SLOT(removeLibDep()));
}

void PluginManager::removeLibDep()
{
    const QObject *o = sender();
    void *handle = NULL;
    if(_libDep.find(o) == _libDep.end()) {
        return;
    }
    handle = _libDep[o];
    _libDep.erase(o);
    if(!handle || _depCnt.find(handle) == _depCnt.end()) {
        return;
    }
    _depCnt[handle]--;
    if(_arrangedDelete && _libDep.empty()) {
        deleteLater();
    }
}

PluginManager::~PluginManager()
{
    for(map<void*, int>::iterator it = _depCnt.begin();
            it != _depCnt.end(); it++)
    {
        DLASSERT(dlclose(it->first), NOOP);
    }
}

END_BIN_NAMESPACE
