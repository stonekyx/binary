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
