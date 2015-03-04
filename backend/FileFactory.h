#ifndef BACKEND_FILE_FACTORY_H
#define BACKEND_FILE_FACTORY_H

#include "common.h"
#include "File.h"

BEGIN_BIN_NAMESPACE(backend)

class FileFactory {
public:
    typedef enum {
        BACKEND_LIBELF,
        BACKEND_ELFSH,
        BACKEND_NUM
    } BackendType;
    FileFactory(BackendType b) : _backend(b) {}
    File *openFile(const char *);
private:
    BackendType _backend;
};

END_BIN_NAMESPACE

#endif
