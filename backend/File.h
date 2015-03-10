#ifndef BACKEND_FILE_H
#define BACKEND_FILE_H

#include <cstdlib>
#include <QtCore/QObject>

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Backend;
class File;

END_BIN_NAMESPACE

class binary::backend::File : public QObject {
    Q_OBJECT
public:
    virtual int getClass() = 0;

    void setBackend(Backend *b) {
        _backend = b;
    }
protected:
    Backend *_backend;

    File() : _backend(NULL) {}
    virtual ~File() {}
};

#endif
