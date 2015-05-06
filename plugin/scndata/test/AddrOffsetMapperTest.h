/*
 * Copyright 2015 KANG Yuxuan
 *
 * AddrOffsetMapperTest.h by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

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
