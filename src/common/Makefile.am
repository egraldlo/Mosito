AM_CPPFLAGS= -fPIC \
-I/home/Casa/Works/tools/Theron-5.01.01/Include

AM_LDFLAGS=-lc -lm -lrt -lpthread -lxs

LDADD = /home/Casa/Works/tools/Theron-5.01.01/Lib/libtherond.a	

noinst_LIBRARIES=libcommon.a
libcommon_a_SOURCES = \
	m_serialization.cpp				m_serialization.h \
	m_tree_node.cpp					m_tree_node.h \
	m_data_type.cpp					m_data_type.h \
	m_thread_pool.cpp     	        m_thread_pool.h \
	m_synchronize.cpp				m_synchronize.h \
	m_smart_ptr.cpp					m_smart_ptr.h \
	m_circle_queue.cpp				m_circle_queue.h \
	m_task.cpp						m_task.h \
	m_affinitizer.cpp				m_affinitizer.h \
	m_trees.cpp						m_trees.h \
	m_buffer.cpp					m_buffer.h \
	m_configuration.cpp				m_configuration.h \
	m_schema.cpp					m_schema.h \
	m_partitioner.cpp				m_partitioner.h \
	m_logging.cpp					m_logging.h \
	m_sender.cpp					m_sender.h \
	m_merger.cpp					m_merger.h \
	m_ahandler.cpp 					m_ahandler.h \
	m_rb_tree.cpp					m_rb_tree.h \
									m_timer.h \
	m_message.cpp					m_message.h
	

