#include <cstdlib>

#include "FileImplLibelf.h"

#include "Backend.h"

using namespace std;

BEGIN_BIN_NAMESPACE(backend)

Backend::Backend() :
    _type(BACKEND_LIBELF),
    _file(NULL)
{
}

Backend::~Backend()
{
    this->disconnect();
    closeFile();
}

void Backend::setBackendType(BackendType type)
{
    _type = type;
}

File *Backend::openFile(const char *name)
{
    closeFile();
    _file = openFilePrivate(name);
    emit fileChanged(_file);
    return _file;
}

File *Backend::getFile()
{
    return _file;
}

void Backend::closeFile()
{
    if(!_file) {
        return;
    }
    closeFilePrivate(&_file);
    emit fileChanged(_file);
}

File *Backend::openFilePrivate(const char *name)
{
    if(BACKEND_LIBELF == _type) {
        File *res = new FileImplLibelf(name, ELF_C_READ);
        if(res && !res->isValid()) {
            closeFilePrivate(&res);
        }
        if(res) {
            res->setBackend(this);
        }
        return res;
    }
    return NULL;
}

void Backend::closeFilePrivate(File **file)
{
    (*file)->arrangeDelete();
    *file = NULL;
}

void Backend::signalFileChange(File *file)
{
    emit fileChanged(file);
}

END_BIN_NAMESPACE
