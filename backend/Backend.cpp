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
    closeFile();
}

void Backend::setBackendType(BackendType type)
{
    _type = type;
}

File *Backend::openFile(const char *name)
{
    closeFile();
    if(BACKEND_LIBELF == _type) {
        _file = new FileImplLibelf(name, ELF_C_READ);
    }
    if(_file) {
        _file->setBackend(this);
    }
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
    if(dynamic_cast<FileImplLibelf*>(_file)) {
        delete dynamic_cast<FileImplLibelf*>(_file);
    }
    _file = NULL;
    emit fileChanged(_file);
}

END_BIN_NAMESPACE
