/*
 * Copyright 2015 KANG Yuxuan
 *
 * DemangleWrapTest.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <cstring>

#include "DemangleWrapTest.h"
#include "DemangleWrapTestExtern.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

CPPUNIT_TEST_SUITE_REGISTRATION( DemangleWrapTest );

void DemangleWrapTest::testNoMangle()
{
    CPPUNIT_ASSERT( !strcmp(cplus_demangle("cplus_demangle"),
                "cplus_demangle") );
    CPPUNIT_ASSERT( !strcmp(cplus_demangle("strlen"),
                "strlen") );
}

void DemangleWrapTest::testNoMangleCutEnd()
{
    CPPUNIT_ASSERT( !strcmp(cplus_demangle("cplus_demangle@plt"),
                "cplus_demangle") );
    CPPUNIT_ASSERT( !strcmp(cplus_demangle("__strcmp@plt"),
                "__strcmp") );
}

void DemangleWrapTest::testMangle()
{
    CPPUNIT_ASSERT( !strcmp(cplus_demangle("_ZN11QStringListC1Ev"),
                "QStringList::QStringList()") );
}

END_PLUG_NAMESPACE
