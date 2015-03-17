#include "wrap.h"
#include <libiberty/demangle.h>

BEGIN_PLUG_NAMESPACE(strtab)

char *cplus_demangle(const char *mangled)
{
    return ::cplus_demangle(mangled, DMGL_AUTO | DMGL_PARAMS);
}

END_PLUG_NAMESPACE
