/*
 * Copyright 2015 KANG Yuxuan
 *
 * CtxJudge.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_FRAMEWORK_CTX_JUDGE_H
#define PLUGIN_FRAMEWORK_CTX_JUDGE_H

#include <cstdlib>

#include "common.h"

class QModelIndex;

BEGIN_PLUG_NAMESPACE(plugin_framework)

class CtxAction;
class CtxJudge {
public:
    typedef bool (*JudgeFunc)(CtxAction *, const QModelIndex &);
    CtxJudge(JudgeFunc = NULL);
    virtual ~CtxJudge();
    virtual bool operator()(CtxAction *, const QModelIndex &);
private:
    JudgeFunc _func;
};

END_PLUG_NAMESPACE

#endif
