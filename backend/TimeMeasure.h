/*
 * Copyright 2015 KANG Yuxuan
 *
 * TimeMeasure.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef BACKEND_TIME_MEASURE_H
#define BACKEND_TIME_MEASURE_H

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class TimeMeasure {
public:
    TimeMeasure();
    ~TimeMeasure();
    bool start();
    bool end();
    double getDiff();
private:
    class Private;
    Private *_priv;
};

END_BIN_NAMESPACE

#endif
