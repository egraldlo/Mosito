AM_CPPFLAGS= -fPIC \
-I/home/Casa/Works/tools/Theron-5.01.01/Include

AM_LDFLAGS=-lc -lm -lrt -lpthread -lxs

LDADD = ../common/libcommon.a
		/home/Casa/Works/tools/Theron-5.01.01/Lib/libtherond.a	

noinst_LIBRARIES=libdeploy.a
libdeploy_a_SOURCES = \
	m_worker.cpp				m_worker.h \
	m_coordinator.cpp			m_coordinator.h
