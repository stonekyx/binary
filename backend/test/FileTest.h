#ifndef BACKEND_TEST_FILE_H
#define BACKEND_TEST_FILE_H

#include <cppunit/extensions/HelperMacros.h>

BEGIN_BIN_NAMESPACE(backend)

class FileTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( FileTest );
    CPPUNIT_TEST( testOpenAR );
    CPPUNIT_TEST( testOpenExec );
    CPPUNIT_TEST( testOpenSO );
    CPPUNIT_TEST_SUITE_END();
public:
    void testOpenAR();
    void testOpenExec();
    void testOpenSO();
};

END_BIN_NAMESPACE

#endif
