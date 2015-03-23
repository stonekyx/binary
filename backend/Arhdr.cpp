#include <libelf.h>
#include <cstring>

#include "Arhdr.h"

BEGIN_BIN_NAMESPACE(backend)

Arhdr::Arhdr(const void *srcVoid, loff_t off) :
    _valid(false), _name(NULL), _date(0), _uid(0), _gid(0), _mode(0),
    _size(0), _rawname(NULL), _offset(off)
{
    Elf_Arhdr *src = (Elf_Arhdr*)srcVoid;
    if(!src) {
        return;
    }
    _valid = true;
    _name = src->ar_name?strdup(src->ar_name):NULL;
    _date = src->ar_date;
    _uid = src->ar_uid;
    _gid = src->ar_gid;
    _mode = src->ar_mode;
    _size = src->ar_size;
    _rawname = src->ar_rawname?strdup(src->ar_rawname):NULL;
}

Arhdr::Arhdr(const Arhdr &o)
{
    *this = o;
}

Arhdr &Arhdr::operator=(const Arhdr &o)
{
    _valid = o._valid;
    _name = o._name?strdup(o._name):NULL;
    _date = o._date;
    _uid = o._uid;
    _gid = o._gid;
    _mode = o._mode;
    _size = o._size;
    _rawname = o._rawname?strdup(o._rawname):NULL;
    _offset = o._offset;
    return *this;
}

Arhdr::~Arhdr()
{
    if(_name) {
        free(_name);
    }
    if(_rawname) {
        free(_rawname);
    }
}

END_BIN_NAMESPACE
