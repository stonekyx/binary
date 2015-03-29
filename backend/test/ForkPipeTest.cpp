#include <cstring>

#include "ForkPipeTest.h"

BEGIN_BIN_NAMESPACE(backend)

CPPUNIT_TEST_SUITE_REGISTRATION( ForkPipeTest );

void ForkPipeTest::testShort()
{
    const char *argv[] = {"echo", "true", NULL};
    ForkPipe forkPipe("echo", argv);
    forkPipe.execAndWait();
    CPPUNIT_ASSERT( !strcmp(forkPipe.getBuf(), "true\n") );
}

void ForkPipeTest::testSleep()
{
    const char *argv[] = {"sleep", "0.5", NULL};
    ForkPipe forkPipe("sleep", argv);
    CPPUNIT_ASSERT( !forkPipe.execAndWait() );
    CPPUNIT_ASSERT( !strcmp(forkPipe.getBuf(), "") );
}

END_BIN_NAMESPACE
