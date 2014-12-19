AM_CPPFLAGS= -fPIC
AM_LDFLAGS=-lc -lm -lrt -lpthread

LDADD = ../common/libcommon.a

noinst_LIBRARIES=libdeploy.a
libdeploy_a_SOURCES = \
	m_worker.cpp				m_worker.h \
	m_coordinator.cpp			m_coordinator.h
