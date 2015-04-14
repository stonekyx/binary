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
