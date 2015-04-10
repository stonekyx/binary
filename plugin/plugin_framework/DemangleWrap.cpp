#include <cstdlib>

#include "DemangleWrap.h"
#ifdef LIBIBERTY_DEMANGLE
#include <libiberty/demangle.h>
#elif DEFAULT_DEMANGLE
#include <demangle.h>
#endif

#include "test/DemangleWrapTestExtern.h"

extern "C" {
    extern char *strdup(const char *);
    extern size_t strlen(const char *s);
    extern int strcmp(const char *s1, const char *s2);
}

BEGIN_PLUG_NAMESPACE(plugin_framework)

ssize_t endsWith(const char *mangled, const char *needle)
{
    size_t mlen = strlen(mangled);
    size_t nlen = strlen(needle);
    if(mlen <= nlen) {
        return false;
    }
    return !strcmp(mangled+mlen-nlen, needle) ? mlen-nlen : -1;
}

static const char *cutEnds[] = {"@plt"};

char *cplus_demangle(const char *mangled)
{
    char *alloc = strdup(mangled);
    for(size_t i=0; i<sizeof(cutEnds)/sizeof(cutEnds[0]); i++) {
        ssize_t cutpos = endsWith(mangled, cutEnds[i]);
        if(cutpos != -1) {
            alloc[cutpos] = 0;
            break;
        }
    }
    char *res = ::cplus_demangle(alloc, DMGL_AUTO | DMGL_PARAMS);
    if(!res) {
        return alloc;
    }
    free(alloc);
    return res;
}

END_PLUG_NAMESPACE
