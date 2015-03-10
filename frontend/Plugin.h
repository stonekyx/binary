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

END_BIN_NAMESPACE

class binary::frontend::Plugin : public QObject {
    Q_OBJECT
public:
    virtual const char *getName() = 0;
    virtual QAction *getAction(QWidget *) = 0;
    void *handle;
public slots:
    virtual void createView() = 0;
    virtual QObject *createView(std::map<std::string, std::string>) = 0;
private slots:
    virtual void removeDeadView(QObject *) = 0;
};

#define DEFINE_PLUGIN(name, viewClass, trContext, title) \
    class Plugin ## name : public BIN_NAMESPACE(frontend)::Plugin { \
    public: \
        Plugin ## name() : _actionPlugin(NULL) {} \
        ~Plugin ## name() { \
            for(std::set<QObject*>::iterator it = _views.begin(); \
                    it != _views.end(); it++) \
            { \
                (*it)->deleteLater(); \
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
            QObject *res = new viewClass(); \
            _views.insert(res); \
            QObject::connect(res, SIGNAL(destroyed(QObject *)), \
                    this, SLOT(removeDeadView(QObject *))); \
        } \
        virtual QObject *createView(std::map<std::string, std::string> param) { \
            QObject *res = new viewClass(param); \
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
    BIN_NAMESPACE(frontend)::Plugin *createPlugin() \
    { \
        return new Plugin ## name; \
    }

#endif
