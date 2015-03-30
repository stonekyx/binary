#ifndef BACKEND_TEST_FORKPIPE_H
#define BACKEND_TEST_FORKPIPE_H

#include <cppunit/extensions/HelperMacros.h>

#include "../ForkPipe.h"

BEGIN_BIN_NAMESPACE(backend)

class ForkPipeTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( ForkPipeTest );
    CPPUNIT_TEST( testShort );
    CPPUNIT_TEST( testSleep );
    CPPUNIT_TEST( testTimeout );
    CPPUNIT_TEST( testInit );
    CPPUNIT_TEST_SUITE_END();
public:
    void testShort();
    void testSleep();
    void testTimeout();
    void testInit();
};

END_BIN_NAMESPACE

#endif
