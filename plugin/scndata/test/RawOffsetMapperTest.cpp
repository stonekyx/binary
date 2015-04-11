#include <QtGui/QTextDocument>

#include "../RawOffsetMapper.h"
#include "RawOffsetMapperTest.h"

BEGIN_PLUG_NAMESPACE(scndata)

CPPUNIT_TEST_SUITE_REGISTRATION( RawOffsetMapperTest );

void RawOffsetMapperTest::testToOffsetNoSel()
{
    int cursor=0, cursorE=0;
    int offset, offsetE;
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 1, offsetE );

    cursor = cursorE = 10;
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 10, offset );
    CPPUNIT_ASSERT_EQUAL( 11, offsetE );

    cursor = cursorE = 16;
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 15, offset );
    CPPUNIT_ASSERT_EQUAL( 16, offsetE );
}

void RawOffsetMapperTest::testToOffsetSel()
{
    int cursor=0, cursorE=1;
    int offset, offsetE;
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 1, offsetE );

    cursor = 1; cursorE = 10;
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 1, offset );
    CPPUNIT_ASSERT_EQUAL( 10, offsetE );

    cursor = 17; cursorE = 18; //first char in second line
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 17, offsetE );

    cursor = 17; cursorE = 33; //all second line
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 32, offsetE );

    cursor = 17; cursorE = 34; //all second line
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 32, offsetE );

    cursor = 17; cursorE = 35; //all second line and first in third line
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 33, offsetE );

    cursor = 15; cursorE = 51; //last in first, all second and third line
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 15, offset );
    CPPUNIT_ASSERT_EQUAL( 48, offsetE );

    cursor = 17; cursorE = 51; //all second and third line
    RawOffsetMapper().toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 16, offset );
    CPPUNIT_ASSERT_EQUAL( 48, offsetE );
}

void RawOffsetMapperTest::testFromOffsetNoSel()
{
    int offset = 0, offsetE = 0;
    int cursor, cursorE;
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 0, cursor );
    CPPUNIT_ASSERT_EQUAL( 0, cursorE );

    offset = offsetE = 1;
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 1, cursor );
    CPPUNIT_ASSERT_EQUAL( 1, cursorE );

    offset = offsetE = 15;
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 15, cursor );
    CPPUNIT_ASSERT_EQUAL( 15, cursorE );

    offset = offsetE = 16;
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 17, cursor );
    CPPUNIT_ASSERT_EQUAL( 17, cursorE );
}

void RawOffsetMapperTest::testFromOffsetSel()
{
    int offset = 0, offsetE = 1;
    int cursor, cursorE;
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 0, cursor );
    CPPUNIT_ASSERT_EQUAL( 1, cursorE );

    offset = 1; offsetE = 5;
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 1, cursor );
    CPPUNIT_ASSERT_EQUAL( 5, cursorE );

    offset = 5; offsetE = 20; //cross line 1 and line 2
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 5, cursor );
    CPPUNIT_ASSERT_EQUAL( 21, cursorE );

    offset = 16; offsetE = 32; //all line 2
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 17, cursor );
    CPPUNIT_ASSERT( 33 == cursorE || 34 == cursorE );

    offset = 16; offsetE = 48; //all line 2
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 17, cursor );
    CPPUNIT_ASSERT( 50 == cursorE || 51 == cursorE );

    offset = 16; offsetE = 49; //all line 2
    RawOffsetMapper().fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 17, cursor );
    CPPUNIT_ASSERT_EQUAL( 52, cursorE );
}

void RawOffsetMapperTest::testToOffsetDocument()
{
    int cursor=19, cursorE=19;
    int offset, offsetE;
    QTextDocument doc("0123456789abcdef\nabc");
    RawOffsetMapper(&doc).toOffset(offset, offsetE,
            cursor, cursorE);
    CPPUNIT_ASSERT_EQUAL( 18, offset );
    CPPUNIT_ASSERT_EQUAL( 19, offsetE );
}

END_PLUG_NAMESPACE
