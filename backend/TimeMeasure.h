#ifndef BACKEND_TIME_MEASURE_H
#define BACKEND_TIME_MEASURE_H

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class TimeMeasure {
public:
    TimeMeasure();
    ~TimeMeasure();
    bool start();
    bool end();
    double getDiff();
private:
    class Private;
    Private *_priv;
};

END_BIN_NAMESPACE

#endif
