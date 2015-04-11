#ifndef PLUGIN_SCNDATA_TEST_RAW_OFFSET_MAPPER_H
#define PLUGIN_SCNDATA_TEST_RAW_OFFSET_MAPPER_H

#include <cppunit/extensions/HelperMacros.h>

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

class RawOffsetMapperTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( RawOffsetMapperTest );
    CPPUNIT_TEST( testToOffsetNoSel );
    CPPUNIT_TEST( testToOffsetSel );
    CPPUNIT_TEST( testFromOffsetNoSel );
    CPPUNIT_TEST( testFromOffsetSel );
    CPPUNIT_TEST( testToOffsetDocument );
    CPPUNIT_TEST_SUITE_END();
public:
    void testToOffsetNoSel();
    void testToOffsetSel();
    void testFromOffsetNoSel();
    void testFromOffsetSel();
    void testToOffsetDocument();
};

END_PLUG_NAMESPACE

#endif
