#ifndef BACKEND_ARSYM_H
#define BACKEND_ARSYM_H

#include <cstdlib>

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Arsym {
public:
    Arsym(const void * = NULL);
    ~Arsym();
    Arsym(const Arsym &);
    Arsym &operator=(const Arsym &);
    bool isValid() { return _valid; }
    const char *name() { return _name; }
    size_t off() { return _off; }
    unsigned long int hash() { return _hash; }
private:
    bool _valid;
    char *_name;
    size_t _off;
    unsigned long int _hash;
};

END_BIN_NAMESPACE

#endif
