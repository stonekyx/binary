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

#endif
