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

void Backend::registerView(Observer *obs)
{
    _observers.push_back(obs);
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
    return _file;
}

File *Backend::getFile()
{
    return _file;
}

void Backend::updateAll()
{
    for(vector<Observer*>::iterator it = _observers.begin();
            it != _observers.end(); it++)
    {
        (*it)->update();
    }
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
}

END_BIN_NAMESPACE
