#ifndef FRONTEND_PLUGIN_H
#define FRONTEND_PLUGIN_H

#include <map>
#include <string>

#include "common.h"

#include <QtGui/QAction>
#include <QtGui/QWidget>
#include <QtCore/QObject>
#include <QtGui/QApplication>

BEGIN_BIN_NAMESPACE(frontend)

class Plugin;
class PluginManager;

END_BIN_NAMESPACE

class binary::frontend::Plugin : public QObject {
    Q_OBJECT
public:
    Plugin(PluginManager *manager) : manager(manager) {}
    virtual ~Plugin() = 0;
    virtual const char *getName() = 0;
    virtual QAction *getAction(QWidget *) = 0;
    void *handle;
    PluginManager * const manager;
public slots:
    virtual void createView() = 0;
    virtual QObject *createView(std::map<std::string, std::string>) = 0;
private slots:
    virtual void removeDeadView(QObject *) = 0;
};

inline binary::frontend::Plugin::~Plugin() { }

#define DEFINE_PLUGIN(name, viewClass, trContext, title) \
    class Plugin ## name : public BIN_NAMESPACE(frontend)::Plugin { \
    public: \
        Plugin ## name(BIN_NAMESPACE(frontend)::PluginManager *manager) : \
            Plugin(manager), \
            _actionPlugin(NULL) \
        {} \
        ~Plugin ## name() { \
            for(std::set<QObject*>::iterator it = _views.begin(); \
                    it != _views.end(); it++) \
            { \
                if(dynamic_cast<QWidget*>(*it)) { \
                    dynamic_cast<QWidget*>(*it)->close(); \
                } else { \
                    (*it)->deleteLater(); \
                } \
            } \
            _views.clear(); \
        } \
        virtual const char *getName() { \
            return #name; \
        } \
        virtual QAction *getAction(QWidget *parent) { \
            if(_actionPlugin) { \
                _actionPlugin->setParent(parent); \
                return _actionPlugin; \
            } \
            _actionPlugin = new QAction(parent); \
            _actionPlugin->setObjectName("_actionPlugin"#name); \
            _actionPlugin->setText(QApplication::translate(trContext, title, 0, QApplication::UnicodeUTF8)); \
            QObject::connect(_actionPlugin, \
                    SIGNAL(triggered()), \
                    this, \
                    SLOT(createView())); \
            return _actionPlugin; \
        } \
        virtual void createView() { \
            QObject *res = new viewClass(this); \
            _views.insert(res); \
            QObject::connect(res, SIGNAL(destroyed(QObject *)), \
                    this, SLOT(removeDeadView(QObject *))); \
        } \
        virtual QObject *createView(std::map<std::string, std::string> param) { \
            QObject *res = new viewClass(this, param); \
            _views.insert(res); \
            QObject::connect(res, SIGNAL(destroyed(QObject *)), \
                    this, SLOT(removeDeadView(QObject *))); \
            return res; \
        } \
    private: \
        void removeDeadView(QObject *view) { \
            _views.erase(view); \
        } \
        QAction *_actionPlugin; \
        std::set<QObject*> _views; \
    }; \
    extern "C" \
    BIN_NAMESPACE(frontend)::Plugin \
    *createPlugin(BIN_NAMESPACE(frontend)::PluginManager *manager) \
    { \
        return new Plugin ## name(manager); \
    }

#endif
