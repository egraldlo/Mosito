AM_CPPFLAGS= -fPIC

AM_LDFLAGS= -lc -lm -lrt

include_HEADERS = m_server.h
				
bin_PROGRAMS = mosito 

mosito_SOURCES = m_server.cpp

mosito_LDADD = executor/libexecutor.a 
		 
SUBDIRS = common executor expressions logical optimizer physical storage sql analysis
DIST_SUBDIRS = common executor expressions logical optimizer physical storage sql analysis
