/*
 * Copyright 2015 KANG Yuxuan
 *
 * common.h by KANG Yuxuan <stonekyx@gmail.com>
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
