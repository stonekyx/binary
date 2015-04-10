#include <ctime>

#include "TimeMeasure.h"

BEGIN_BIN_NAMESPACE(backend)

class TimeMeasure::Private {
public:
    struct timespec _startTime;
    struct timespec _endTime;
    bool _started, _ended;
};

TimeMeasure::TimeMeasure()
{
    _priv = new Private();
    _priv->_started = _priv->_ended = false;
}

TimeMeasure::~TimeMeasure()
{
    delete _priv;
}

bool TimeMeasure::start()
{
    _priv->_ended = false;
    if(clock_gettime(CLOCK_MONOTONIC, &_priv->_startTime) == 0) {
        _priv->_started = true;
        return true;
    }
    return false;
}

bool TimeMeasure::end()
{
    if(clock_gettime(CLOCK_MONOTONIC, &_priv->_endTime) == 0) {
        _priv->_ended = true;
        return true;
    }
    return false;
}

static double toDouble(struct timespec *ts)
{
    return ts->tv_sec + ts->tv_nsec*1e-9;
}

double TimeMeasure::getDiff()
{
    if(!_priv->_started || !_priv->_ended) {
        return -1;
    }
    return toDouble(&_priv->_endTime) - toDouble(&_priv->_startTime);
}

END_BIN_NAMESPACE
