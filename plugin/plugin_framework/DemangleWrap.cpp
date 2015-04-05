#include "DemangleWrap.h"
#ifdef LIBIBERTY_DEMANGLE
#include <libiberty/demangle.h>
#elif DEFAULT_DEMANGLE
#include <demangle.h>
#endif

extern "C" {
    extern char *strdup(const char *);
}

BEGIN_PLUG_NAMESPACE(plugin_framework)

char *cplus_demangle(const char *mangled)
{
    char *res = ::cplus_demangle(mangled, DMGL_AUTO | DMGL_PARAMS);
    if(!res) {
        return strdup(mangled);
    }
    return res;
}

END_PLUG_NAMESPACE
