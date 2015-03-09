#ifndef FRONTEND_PLUGINVIEW_H
#define FRONTEND_PLUGINVIEW_H

#include <map>
#include <string>

#include "common.h"
#include "backend/Observer.h"

BEGIN_BIN_NAMESPACE(frontend)

class PluginView : public BIN_NAMESPACE(backend)::Observer {
public:
    PluginView() {}
    virtual ~PluginView() = 0;
};

inline PluginView::~PluginView()
{
    //TODO: inform owner of the closing.
}

END_BIN_NAMESPACE

#endif
