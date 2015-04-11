#include <QtGui/QTextDocument>
#include <QtGui/QTextCursor>

#include "../AddrOffsetMapper.h"
#include "AddrOffsetMapperTest.h"

BEGIN_PLUG_NAMESPACE(scndata)

CPPUNIT_TEST_SUITE_REGISTRATION( AddrOffsetMapperTest );

void AddrOffsetMapperTest::testCursorBehavior()
{
    QTextDocument doc("0x1\n0x23\n0x456");
    QTextCursor cursor(&doc);
    cursor.setPosition(0);
    CPPUNIT_ASSERT_EQUAL( 0, cursor.blockNumber() );
    cursor.setPosition(3);
    CPPUNIT_ASSERT_EQUAL( 0, cursor.blockNumber() );
    cursor.setPosition(4);
    CPPUNIT_ASSERT_EQUAL( 1, cursor.blockNumber() );
    cursor.setPosition(7);
    CPPUNIT_ASSERT_EQUAL( 1, cursor.blockNumber() );
    cursor.setPosition(8);
    CPPUNIT_ASSERT_EQUAL( 1, cursor.blockNumber() );
    cursor.setPosition(9);
    CPPUNIT_ASSERT_EQUAL( 2, cursor.blockNumber() );
    cursor.setPosition(13);
    CPPUNIT_ASSERT_EQUAL( 2, cursor.blockNumber() );
    cursor.setPosition(14);
    CPPUNIT_ASSERT_EQUAL( 2, cursor.blockNumber() );
}

void AddrOffsetMapperTest::testToOffset()
{
    QTextDocument doc("0x1\n0x23\n0x456");
    QTextCursor cursor(&doc);
    cursor.setPosition(1);
    QTextCursor cursorE(&doc);
    cursorE.setPosition(6);
    int offset, offsetE;
    AddrOffsetMapper(&doc).toOffset(offset, offsetE,
            cursor.position(), cursorE.position());
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 32, offsetE );

    cursorE.setPosition(1);
    AddrOffsetMapper(&doc).toOffset(offset, offsetE,
            cursor.position(), cursorE.position());
    CPPUNIT_ASSERT_EQUAL( 0, offset );
    CPPUNIT_ASSERT_EQUAL( 16, offsetE );
}

void AddrOffsetMapperTest::testFromOffset()
{
    QTextDocument doc("0x1\n0x23\n0x4567\n0x1234567");
    int offset = 0, offsetE = 4;
    int cursor, cursorE;
    AddrOffsetMapper(&doc).fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 0, cursor );
    CPPUNIT_ASSERT_EQUAL( 3, cursorE );

    offset = 20; offsetE = 34;
    AddrOffsetMapper(&doc).fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 4, cursor );
    CPPUNIT_ASSERT_EQUAL( 15, cursorE );

    offset = 16; offsetE = 32;
    AddrOffsetMapper(&doc).fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 4, cursor );
    CPPUNIT_ASSERT_EQUAL( 8, cursorE );

    offset = 63; offsetE = 64;
    AddrOffsetMapper(&doc).fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 16, cursor );
    CPPUNIT_ASSERT_EQUAL( 25, cursorE );

    offset = 0; offsetE = 64;
    AddrOffsetMapper(&doc).fromOffset(cursor, cursorE,
            offset, offsetE);
    CPPUNIT_ASSERT_EQUAL( 0, cursor );
    CPPUNIT_ASSERT_EQUAL( 25, cursorE );
}

END_PLUG_NAMESPACE
