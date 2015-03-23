#include <libelf.h>
#include <cstring>

#include "Arsym.h"

BEGIN_BIN_NAMESPACE(backend)

Arsym::Arsym(const void *srcVoid) :
    _valid(false), _name(NULL), _off(0), _hash(0)
{
    Elf_Arsym *src = (Elf_Arsym*)srcVoid;
    if(!src) {
        return;
    }
    _valid = true;
    _name = src->as_name?strdup(src->as_name):NULL;
    _off = src->as_off;
    _hash = src->as_hash;
}

Arsym::Arsym(const Arsym &o)
{
    *this = o;
}

Arsym &Arsym::operator=(const Arsym &o)
{
    _valid = o._valid;
    _name = o._name?strdup(o._name):NULL;
    _off = o._off;
    _hash = o._hash;
    return *this;
}

Arsym::~Arsym()
{
    if(_name) {
        free(_name);
    }
}

END_BIN_NAMESPACE
