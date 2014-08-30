AM_CPPFLAGS= -fPIC 

AM_LDFLAGS=-lc -lm -lrt -lpthread -ll -ly

noinst_LIBRARIES=libsqlparser.a
libsqlparser_a_SOURCES = \
	m_sql_parser.cpp				m_sql_parser.h \
	function.cpp \
	lex.yy.cpp \
	sql.tab.cpp 					sql.tab.hpp \
	sql_node_struct.h \
	runparsertree.cpp			
