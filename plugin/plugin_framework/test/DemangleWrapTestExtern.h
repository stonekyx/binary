#ifndef PLUGIN_FRAMEWORK_TEST_DEMANGLEWRAP_EXTERN_H
#define PLUGIN_FRAMEWORK_TEST_DEMANGLEWRAP_EXTERN_H

#include "common.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

ssize_t endsWith(const char *mangled, const char *needle);

char *cplus_demangle(const char *);

END_PLUG_NAMESPACE

#endif
