AM_CPPFLAGS= -fPIC

AM_LDFLAGS= -lc -lm -lrt

include_HEADERS = m_test.h
				
bin_PROGRAMS = test 

test_SOURCES = m_test.cpp

test_LDADD = common/libtest_common.a \
				 ../src/expressions/libexpressions.a \
				 ../src/physical/libphysical.a \
				 ../src/common/libcommon.a
		 
SUBDIRS = common executor expressions logical optimizer physical storage
DIST_SUBDIRS = common executor expressions logical optimizer physical storage 
