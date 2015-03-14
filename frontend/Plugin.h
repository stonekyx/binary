#ifndef FRONTEND_PLUGIN_H
#define FRONTEND_PLUGIN_H

#include <map>
#include <string>

#include "common.h"

#include <QtGui/QAction>
#include <QtGui/QWidget>
#include <QtCore/QObject>

BEGIN_BIN_NAMESPACE(frontend)

class Plugin;
class PluginManager;

END_BIN_NAMESPACE

class binary::frontend::Plugin : public QObject {
    Q_OBJECT
public:
    Plugin() {}
    virtual ~Plugin() = 0;
    virtual const char *getName() = 0;
    virtual QAction *getAction(QWidget *) = 0;
    void *handle;
    PluginManager *manager;
public slots:
    virtual void createView() = 0;
    virtual QObject *createView(std::map<std::string, std::string>) = 0;
protected slots:
    virtual void removeDeadView(QObject *) = 0;
};

inline binary::frontend::Plugin::~Plugin() { }

#endif
