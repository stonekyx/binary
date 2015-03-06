#ifndef BACKEND_OBSERVER_H
#define BACKEND_OBSERVER_H

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Observer {
public:
    virtual void update() = 0;
};

END_BIN_NAMESPACE

#endif
