#ifndef FRONTEND_CONCRETE_PLUGIN_H
#define FRONTEND_CONCRETE_PLUGIN_H

#include <set>

#include <QtCore/QString>
#include <QtGui/QApplication>

#include "common.h"
#include "Plugin.h"

BEGIN_BIN_NAMESPACE(frontend)

template<typename ViewClass>
class ConcretePlugin : public Plugin {
public:
    ConcretePlugin(const char *name,
            const char *title,
            const char *trContext) :
        _actionPlugin(NULL),
        _name(name),
        _title(title),
        _trContext(trContext)
    {
        if(strchr(name, ' ')) {
            throw std::exception();
        }
    }
    ~ConcretePlugin() {
        for(std::set<QObject*>::iterator it = _views.begin();
                it != _views.end(); it++)
        {
            if(dynamic_cast<QWidget*>(*it)) {
                dynamic_cast<QWidget*>(*it)->close();
            } else {
                (*it)->deleteLater();
            }
        }
        _views.clear();
    }
    virtual const char *getName() {
        return _name;
    }
    virtual QAction *getAction(QWidget *parent) {
        if(_actionPlugin) {
            _actionPlugin->setParent(parent);
            return _actionPlugin;
        }
        _actionPlugin = new QAction(parent);
        _actionPlugin->setObjectName(QString("_actionPlugin%1").arg(_name));
        _actionPlugin->setText(QApplication::translate(_trContext, _title, 0, QApplication::UnicodeUTF8));
        QObject::connect(_actionPlugin,
                SIGNAL(triggered()),
                this,
                SLOT(createView()));
        return _actionPlugin;
    }
    virtual void createView() {
        createView(std::map<std::string, std::string>());
    }
    virtual QObject *createView(std::map<std::string, std::string> param) {
        QObject *res = new ViewClass(this, param);
        _views.insert(res);
        QObject::connect(res, SIGNAL(destroyed(QObject *)),
                this, SLOT(removeDeadView(QObject *)));
        return res;
    }
protected:
    void removeDeadView(QObject *view) {
        _views.erase(view);
    }
private:
    QAction *_actionPlugin;
    std::set<QObject*> _views;
    const char *_name, *_title, *_trContext;
};

END_BIN_NAMESPACE

#define DEFINE_PLUGIN(viewClass, name, title, trContext) \
    extern "C" \
    BIN_NAMESPACE(frontend)::Plugin \
    *createPlugin(BIN_NAMESPACE(frontend)::PluginManager *manager) \
    { \
        BIN_NAMESPACE(frontend)::Plugin *plugin = \
            new BIN_NAMESPACE(frontend)::ConcretePlugin<viewClass>(name, title, trContext); \
        plugin->manager = manager; \
        return plugin; \
    }

#endif
