#ifndef PLUGIN_SCNDATA_DATAVALUE_CONVERT_TYPES_H
#define PLUGIN_SCNDATA_DATAVALUE_CONVERT_TYPES_H

#include <sstream>
#include <iomanip>
#include "DataValueConvert.h"

BEGIN_PLUG_NAMESPACE(scndata)

template<typename T>
class DataValueConvertTypes : public DataValueConvert {
public:
    virtual ~DataValueConvertTypes() {}
    virtual QString toString(const char *data, size_t size)
    {
        char *buf = new char[sizeof(T)];
        memset(buf, 0, sizeof(T));
        memcpy(buf, data, size>sizeof(T)?sizeof(T):size);
        T val;
        memcpy(&val, buf, sizeof(T));
        delete[] buf;
        std::stringstream ss;
        ss << std::setprecision(16) << +val;
        return QString::fromStdString(ss.str());
    }
};

template<>
class DataValueConvertTypes<char*> : public DataValueConvert {
    virtual ~DataValueConvertTypes() {}
    virtual QString toString(const char *data, size_t size)
    {
        QString res;
        for(size_t i=0; i<size; i++) {
            if(data[i] && isprint(data[i])) {
                res += data[i];
            } else {
                break;
            }
        }
        return res;
    }
};

END_PLUG_NAMESPACE

#endif
