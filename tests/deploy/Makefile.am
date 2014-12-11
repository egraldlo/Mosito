AM_CPPFLAGS= -fPIC 

AM_LDFLAGS=-lc -lm -lrt -lpthread

LD_ADD = ../../src/deploy/libdeploy.a

noinst_LIBRARIES=libtest_deploy.a
libtest_deploy_a_SOURCES = \
		m_worker_test.cpp \
		m_coordinator_test.cpp