#ifndef PLUGIN_FRAMEWORK_TEST_DEMANGLEWRAP_H
#define PLUGIN_FRAMEWORK_TEST_DEMANGLEWRAP_H

#include <cppunit/extensions/HelperMacros.h>

#include "common.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

class DemangleWrapTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( DemangleWrapTest );
    CPPUNIT_TEST( testNoMangle );
    CPPUNIT_TEST( testNoMangleCutEnd );
    CPPUNIT_TEST( testMangle );
    CPPUNIT_TEST_SUITE_END();
public:
    void testNoMangle();
    void testNoMangleCutEnd();
    void testMangle();
};

END_PLUG_NAMESPACE

#endif
