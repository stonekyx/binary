#ifndef PLUGIN_SCNDATA_DATAVALUE_CONVERT_H
#define PLUGIN_SCNDATA_DATAVALUE_CONVERT_H

#include <QtCore/QString>

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

class DataValueConvert {
public:
    virtual ~DataValueConvert() = 0;
    virtual QString toString(const char *, size_t) = 0;
};

inline DataValueConvert::~DataValueConvert() {}

END_PLUG_NAMESPACE

#endif
