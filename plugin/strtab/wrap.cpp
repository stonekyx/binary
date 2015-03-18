#include "wrap.h"
#ifdef LIBIBERTY_DEMANGLE
#include <libiberty/demangle.h>
#elif DEFAULT_DEMANGLE
#include <demangle.h>
#endif

BEGIN_PLUG_NAMESPACE(strtab)

char *cplus_demangle(const char *mangled)
{
    return ::cplus_demangle(mangled, DMGL_AUTO | DMGL_PARAMS);
}

END_PLUG_NAMESPACE
