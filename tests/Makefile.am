AM_CPPFLAGS= -fPIC \
-I/home/Casa/Works/tools/Theron-5.01.01/Include

AM_LDFLAGS= -lc -lm -lrt -lxs

include_HEADERS = m_test.h
				
bin_PROGRAMS = test 

test_SOURCES = m_test.cpp

test_LDADD =     \
				 ../src/expressions/libexpressions.a \
				 ../src/physical/libphysical.a \
				 ../src/deploy/libdeploy.a \
				 ../src/common/libcommon.a \
				 ../src/sql/libsqlparser.a \
				 ../src/analysis/libanalysis.a \
				 /home/Casa/Works/tools/Theron-5.01.01/Lib/libtherond.a		 
		 
SUBDIRS = common executor expressions logical optimizer physical storage sql analysis deploy
DIST_SUBDIRS = common executor expressions logical optimizer physical storage sql analysis deploy 
