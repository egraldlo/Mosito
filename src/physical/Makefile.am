AM_CPPFLAGS= -fPIC 

AM_LDFLAGS=-lc -lm -lrt -lpthread -lboost_serialization

LDADD = ../common/libcommon.a \
		../executor/libexecutor.a

noinst_LIBRARIES=libphysical.a
libphysical_a_SOURCES = \
	m_query_plan.cpp        m_query_plan.h \
	m_join.cpp				m_join.h \
	m_scan.cpp				m_scan.h \
	m_project.cpp			m_project.h \
	m_debug.cpp				m_debug.h \
	m_sort.cpp				m_sort.h \
	m_shuffle_upper.cpp		m_shuffle_upper.h \
	m_shuffle_lower.cpp		m_shuffle_lower.h \
	m_bcshuffle_upper.cpp   m_bcshuffle_upper.h \
	m_bcshuffle_lower.cpp	m_bcshuffle_lower.h \
	m_srshuffle_upper.cpp	m_srshuffle_upper.h \
	m_srshuffle_lower.cpp	m_srshuffle_lower.h
