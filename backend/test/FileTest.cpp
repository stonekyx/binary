/*
 * Copyright 2015 KANG Yuxuan
 *
 * FileTest.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <cstring>

#include "../TimeMeasure.h"
#include "../Backend.h"
#include "../File.h"

#include "FileTest.h"

BEGIN_BIN_NAMESPACE(backend)

CPPUNIT_TEST_SUITE_REGISTRATION( FileTest );

void FileTest::testOpenAR()
{
    Backend backend;
    TimeMeasure tm;
    tm.start();
    File *file = backend.openFile("_build/test/libtest.a");
    tm.end();
    CPPUNIT_ASSERT( tm.getDiff() < 0.1 );
    CPPUNIT_ASSERT( file );
    CPPUNIT_ASSERT_EQUAL( backend.getFile(), file );
    CPPUNIT_ASSERT( file->isValid() );
}

void FileTest::testOpenExec()
{
    Backend backend;
    TimeMeasure tm;
    tm.start();
    File *file = backend.openFile("_build/backend/test/unittest");
    tm.end();
    CPPUNIT_ASSERT( tm.getDiff() < 0.1 );
    CPPUNIT_ASSERT( file );
    CPPUNIT_ASSERT_EQUAL( backend.getFile(), file );
    CPPUNIT_ASSERT( file->isValid() );
}

void FileTest::testOpenSO()
{
    Backend backend;
    TimeMeasure tm;
    tm.start();
    File *file = backend.openFile("_build/backend/libbackend.so");
    tm.end();
    CPPUNIT_ASSERT( tm.getDiff() < 0.1 );
    CPPUNIT_ASSERT( file );
    CPPUNIT_ASSERT_EQUAL( backend.getFile(), file );
    CPPUNIT_ASSERT( file->isValid() );
}

END_BIN_NAMESPACE
