/*
 * Copyright 2015 KANG Yuxuan
 *
 * Backend.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <cstdlib>

#include "FileImplLibelf.h"

#include "Backend.h"

using namespace std;

BEGIN_BIN_NAMESPACE(backend)

Backend::Backend() :
    _type(BACKEND_LIBELF),
    _file(NULL)
{
}

Backend::~Backend()
{
    this->disconnect();
    closeFile();
}

void Backend::setBackendType(BackendType type)
{
    _type = type;
}

File *Backend::openFile(const char *name)
{
    closeFile();
    _file = openFilePrivate(name);
    emit fileChanged(_file);
    return _file;
}

File *Backend::getFile()
{
    return _file;
}

void Backend::closeFile()
{
    if(!_file) {
        return;
    }
    closeFilePrivate(&_file);
    emit fileChanged(_file);
}

File *Backend::openFilePrivate(const char *name)
{
    if(BACKEND_LIBELF == _type) {
        File *res = new FileImplLibelf(name, ELF_C_READ);
        if(res && !res->isValid()) {
            closeFilePrivate(&res);
        }
        if(res) {
            res->setBackend(this);
        }
        return res;
    }
    return NULL;
}

void Backend::closeFilePrivate(File **file)
{
    (*file)->arrangeDelete();
    *file = NULL;
}

void Backend::signalFileChange(File *file)
{
    emit fileChanged(file);
}

END_BIN_NAMESPACE
