#ifndef PLUGIN_BASIC_VIEW_H
#define PLUGIN_BASIC_VIEW_H

#include <cstdlib>

#include "common.h"
#include "backend/Observer.h"
#include "MainWindow.h"

BEGIN_PLUG_NAMESPACE(basic)

class View : public BIN_NAMESPACE(backend)::Observer {
public:
    ~View();
    void init(QWidget *parent = NULL);
    void deinit();
    virtual void update();

    static View instance;
    MainWindow *_mainWindow;
protected:
    View();
};

END_PLUG_NAMESPACE

#endif
