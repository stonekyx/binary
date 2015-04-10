#ifndef PLUGIN_SCNDATA_TEST_ADDR_OFFSET_MAPPER_H
#define PLUGIN_SCNDATA_TEST_ADDR_OFFSET_MAPPER_H

#include <cppunit/extensions/HelperMacros.h>

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

class AddrOffsetMapperTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( AddrOffsetMapperTest );
    CPPUNIT_TEST( testCursorBehavior );
    CPPUNIT_TEST( testToOffset );
    CPPUNIT_TEST( testFromOffset );
    CPPUNIT_TEST_SUITE_END();
public:
    void testCursorBehavior();
    void testToOffset();
    void testFromOffset();
};

END_PLUG_NAMESPACE

#endif
