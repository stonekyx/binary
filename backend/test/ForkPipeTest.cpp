#include <cstring>

#include "../TimeMeasure.h"

#include "ForkPipeTest.h"

BEGIN_BIN_NAMESPACE(backend)

CPPUNIT_TEST_SUITE_REGISTRATION( ForkPipeTest );

void ForkPipeTest::testShort()
{
    const char *argv[] = {"echo", "true", NULL};
    ForkPipe forkPipe("echo", argv);
    TimeMeasure tm;
    tm.start();
    CPPUNIT_ASSERT_EQUAL( 0, forkPipe.execAndWait() );
    tm.end();
    CPPUNIT_ASSERT( tm.getDiff() < 0.1 );
    CPPUNIT_ASSERT( !strcmp(forkPipe.getBuf(), "true\n") );
}

void ForkPipeTest::testSleep()
{
    const char *argv[] = {"sleep", "0.5", NULL};
    ForkPipe forkPipe("sleep", argv);
    TimeMeasure tm;
    tm.start();
    CPPUNIT_ASSERT_EQUAL( 0, forkPipe.execAndWait() );
    tm.end();
    CPPUNIT_ASSERT( tm.getDiff() <= 0.6 );
    CPPUNIT_ASSERT( !strcmp(forkPipe.getBuf(), "") );
}

void ForkPipeTest::testTimeout()
{
    const char *argv[] = {"sleep", "10", NULL};
    ForkPipe forkPipe("sleep", argv);
    TimeMeasure tm;
    tm.start();
    CPPUNIT_ASSERT_EQUAL( -1, forkPipe.execAndWait() );
    tm.end();
    CPPUNIT_ASSERT( tm.getDiff() <= 1.1 );
    CPPUNIT_ASSERT( !strcmp(forkPipe.getBuf(), "") );
}

void ForkPipeTest::testInit()
{
    const char *argv[] = {"10", NULL};
    ForkPipe forkPipe("", argv);
    CPPUNIT_ASSERT( !strcmp(forkPipe._path, "") );
    CPPUNIT_ASSERT( !strcmp(forkPipe._argv[0], "10") );
    CPPUNIT_ASSERT_EQUAL( (char*)0, forkPipe._argv[1] );
}

END_BIN_NAMESPACE
