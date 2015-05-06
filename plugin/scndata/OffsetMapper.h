/*
 * Copyright 2015 KANG Yuxuan
 *
 * OffsetMapper.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_SCNDATA_OFFSET_MAPPER_H
#define PLUGIN_SCNDATA_OFFSET_MAPPER_H

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

class OffsetMapper {
public:
    virtual ~OffsetMapper() = 0;
    virtual void toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd) = 0;
    virtual void fromOffset(int &curStart, int &curEnd,
            int offStart, int offEnd) = 0;
};

inline OffsetMapper::~OffsetMapper() {}

END_PLUG_NAMESPACE

#endif
