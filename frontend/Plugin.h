#ifndef FRONTEND_PLUGIN_H
#define FRONTEND_PLUGIN_H

#include <map>
#include <string>

#include "common.h"
#include "PluginView.h"

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
    virtual QAction *createAction(QWidget *) = 0;
    void *handle;
public slots:
    virtual void createView() = 0;
    virtual PluginView *createView(std::map<std::string, std::string>) = 0;
};

#define DEFINE_PLUGIN(name, viewClass, trContext, title) \
    class Plugin ## name : public BIN_NAMESPACE(frontend)::Plugin { \
    public: \
        virtual const char *getName() { \
            return #name; \
        } \
        virtual QAction *createAction(QWidget *parent) { \
            QAction *actionPlugin = new QAction(parent); \
            actionPlugin->setObjectName("actionPlugin"#name); \
            actionPlugin->setText(QApplication::translate(trContext, title, 0, QApplication::UnicodeUTF8)); \
            QObject::connect(actionPlugin, \
                    SIGNAL(triggered()), \
                    this, \
                    SLOT(createView())); \
            return actionPlugin; \
        } \
        virtual void createView() { \
            new viewClass(); \
        } \
        virtual BIN_NAMESPACE(frontend)::PluginView *createView(std::map<std::string, std::string> param) { \
            return new viewClass(param); \
        } \
    }; \
    extern "C" \
    BIN_NAMESPACE(frontend)::Plugin *createPlugin() \
    { \
        return new Plugin ## name; \
    }

#endif
