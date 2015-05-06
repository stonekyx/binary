/*
 * Copyright 2015 KANG Yuxuan
 *
 * DataValueConvertTypes.h by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

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
