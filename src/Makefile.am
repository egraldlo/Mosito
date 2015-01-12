AM_CPPFLAGS= -fPIC

AM_LDFLAGS= -lc -lm -lrt -pthreads -lboost_serialization

include_HEADERS = m_server.h
				
bin_PROGRAMS = mosito 

mosito_SOURCES = m_server.cpp

mosito_LDADD = executor/libexecutor.a 
		 
SUBDIRS = common executor expressions logical optimizer physical storage sql analysis deploy
DIST_SUBDIRS = common executor expressions logical optimizer physical storage sql analysis deploy
