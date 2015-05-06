/*
 * Copyright 2015 KANG Yuxuan
 *
 * Backend.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef BACKEND_BACKEND_H
#define BACKEND_BACKEND_H

#include <vector>
#include <QtCore/QObject>

#include "File.h"
#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Backend;

END_BIN_NAMESPACE

class binary::backend::Backend : public QObject {
    Q_OBJECT
public:
    typedef enum {
        BACKEND_LIBELF,
        BACKEND_ELFSH,
        BACKEND_NUM
    } BackendType;

    Backend();
    ~Backend();
    void setBackendType(BackendType);
    File *openFile(const char *);
    void closeFile();
    File *getFile();
    File *openFilePrivate(const char *);
    void closeFilePrivate(File **);
    void signalFileChange(File*);
signals:
    void fileChanged(binary::backend::File *);
private:
    BackendType _type;
    File *_file;
};

#endif
