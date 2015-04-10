#include "../HexOffsetMapper.h"
#include "HexOffsetMapperTest.h"

BEGIN_PLUG_NAMESPACE(scndata)

CPPUNIT_TEST_SUITE_REGISTRATION( HexOffsetMapperTest );

void HexOffsetMapperTest::testToOffsetNoSel()
{
    int cursor=0, cursorE=0;
    int offset, offsetE;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 1, offsetE );

    cursor = cursorE = 10;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 3, offset );
    CPPUNIT_ASSERT_EQUAL( 4, offsetE );

    cursor = cursorE = 17;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 5, offset );
    CPPUNIT_ASSERT_EQUAL( 6, offsetE );

    cursor = cursorE = 18;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 6, offset );
    CPPUNIT_ASSERT_EQUAL( 7, offsetE );

    cursor = cursorE = 47;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 15, offset );
    CPPUNIT_ASSERT_EQUAL( 16, offsetE );

    cursor = cursorE = 48;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 17, offsetE );
}

void HexOffsetMapperTest::testToOffsetSel()
{
    int cursor=0, cursorE=1;
    int offset, offsetE;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 1, offsetE );

    cursor = 1; cursorE = 11;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 4, offsetE );

    cursor = 1; cursorE = 10;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 4, offsetE );

    cursor = 1; cursorE = 9;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 3, offsetE );

    cursor = 2; cursorE = 9;
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 1, offset );
    CPPUNIT_ASSERT_EQUAL( 3, offsetE );

    cursor = 48; cursorE = 49; //first char in second line
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 17, offsetE );

    cursor = 48; cursorE = 95; //all second line
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 32, offsetE );

    cursor = 48; cursorE = 96; //all second line
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 32, offsetE );

    cursor = 49; cursorE = 97; //all second line and first in third line
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 33, offsetE );

    cursor = 45; cursorE = 143; //last in first, all second and third line
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 15, offset );
    CPPUNIT_ASSERT_EQUAL( 48, offsetE );

    cursor = 47; cursorE = 144; //all second and third line
    HexOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 48, offsetE );
}

void HexOffsetMapperTest::testFromOffsetNoSel()
{
    int offset = 0, offsetE = 0;
    int cursor, cursorE;
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 0, cursor );
    CPPUNIT_ASSERT_EQUAL( 0, cursorE );

    offset = offsetE = 1;
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 3, cursor );
    CPPUNIT_ASSERT_EQUAL( 3, cursorE );

    offset = offsetE = 15;
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 45, cursor );
    CPPUNIT_ASSERT_EQUAL( 45, cursorE );

    offset = offsetE = 16;
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 48, cursor );
    CPPUNIT_ASSERT_EQUAL( 48, cursorE );
}

void HexOffsetMapperTest::testFromOffsetSel()
{
    int offset = 0, offsetE = 1;
    int cursor, cursorE;
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 0, cursor );
    CPPUNIT_ASSERT_EQUAL( 2, cursorE );

    offset = 1; offsetE = 5;
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 3, cursor );
    CPPUNIT_ASSERT_EQUAL( 14, cursorE );

    offset = 5; offsetE = 20; //cross line 1 and line 2
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 15, cursor );
    CPPUNIT_ASSERT_EQUAL( 59, cursorE );

    offset = 16; offsetE = 32; //all line 2
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 48, cursor );
    CPPUNIT_ASSERT_EQUAL( 95, cursorE );

    offset = 16; offsetE = 48; //all line 2 and 3
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 48, cursor );
    CPPUNIT_ASSERT_EQUAL( 143, cursorE );

    offset = 16; offsetE = 49; //all line 2, 3 and first in line 4
    HexOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 48, cursor );
    CPPUNIT_ASSERT_EQUAL( 146, cursorE );
}

END_PLUG_NAMESPACE
