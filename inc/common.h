#ifndef INC_COMMON_H
#define INC_COMMON_H

#define BEGIN_BIN_NAMESPACE(x) \
    namespace binary { namespace x {

#define END_BIN_NAMESPACE \
    } }

#define USE_BIN_NAMESPACE(x) \
    using namespace binary::x

#define BIN_NAMESPACE(x) \
    binary::x

#define BEGIN_PLUG_NAMESPACE(x) \
    BEGIN_BIN_NAMESPACE(plugin) namespace x {

#define END_PLUG_NAMESPACE \
    END_BIN_NAMESPACE }

#define PLUG_NAMESPACE(x) \
    BIN_NAMESPACE(plugin)::x

#define USE_PLUG_NAMESPACE(x) \
    using namespace PLUG_NAMESPACE(x)

#endif
