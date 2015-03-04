#ifndef BACKEND_FILE_H
#define BACKEND_FILE_H

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class File {
public:
    virtual int getClass() = 0;
    virtual ~File() {};
protected:
    File() {}
};

END_BIN_NAMESPACE

#endif
