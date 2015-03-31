#ifndef FRONTEND_PLUGIN_MANAGER_H
#define FRONTEND_PLUGIN_MANAGER_H

#include <vector>

#include <QtGui/QAction>

#include "common.h"
#include "backend/Backend.h"

BEGIN_BIN_NAMESPACE(frontend)

class Plugin;
class PluginManager;

END_BIN_NAMESPACE

class binary::frontend::PluginManager : public QObject {
    Q_OBJECT
public:
    PluginManager(BIN_NAMESPACE(backend)::Backend *);
    int loadPlugin(const char *);
    QAction *getAction(int, QWidget *);
    void *findSymbol(int, const char *);
    Plugin *getPlugin(const char *);
    Plugin *getPlugin(int);
    BIN_NAMESPACE(backend)::Backend *getBackend();
    void arrangeDelete();
    void registerLibDep(const QObject *, void *);
protected slots:
    void removeLibDep();
private:
    std::vector<Plugin*> _plugins;
    BIN_NAMESPACE(backend)::Backend *_backend;
    std::map<const QObject *, void *> _libDep;
    std::map<void *, int> _depCnt;
    bool _arrangedDelete;
    ~PluginManager();
};

#endif
