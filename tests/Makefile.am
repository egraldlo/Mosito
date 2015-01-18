AM_CPPFLAGS= -fPIC 

AM_LDFLAGS= -lc -lm -lrt -lxs -lpthread -lboost_serialization -lconfig++

include_HEADERS = m_test.h
				
bin_PROGRAMS = test 

test_SOURCES = m_test.cpp			

test_LDADD =       \
				 ../src/expressions/libexpressions.a \
				 ../src/physical/libphysical.a \
				 ../src/executor/libexecutor.a \
				 ../src/analysis/libanalysis.a \
				 ../src/storage/libstorage.a \
				 ../src/deploy/libdeploy.a \
				 ../src/common/libcommon.a \
				 ../src/sql/libsqlparser.a \
				 ../libs/libtherond.a		 
		 
SUBDIRS = common executor expressions logical optimizer physical storage sql analysis deploy
DIST_SUBDIRS = common executor expressions logical optimizer physical storage sql analysis deploy 
