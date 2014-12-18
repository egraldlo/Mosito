AM_CPPFLAGS= -fPIC \
-I/home/Casa/Works/tools/Theron-5.01.01/Include

AM_LDFLAGS=-lc -lm -lrt -lpthread -lxs

LD_ADD = ../../src/deploy/libdeploy.a

noinst_LIBRARIES=libtest_deploy.a
libtest_deploy_a_SOURCES = \
		m_worker_test.cpp \
		m_coordinator_test.cpp