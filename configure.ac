AC_INIT([Mosito], [0.1], [zhangleicasa@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_LIBTOOL
CPPFLAGS="-w -O0"
CXXFLAGS=${CXXFLAGS="-g"}
THERON_XS=1
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/common/Makefile
  src/executor/Makefile
  src/expressions/Makefile
  src/logical/Makefile
  src/optimizer/Makefile  
  src/physical/Makefile
  src/storage/Makefile
  src/sql/Makefile
  src/analysis/Makefile
  src/deploy/Makefile
  tests/Makefile
  tests/common/Makefile
  tests/executor/Makefile
  tests/expressions/Makefile
  tests/logical/Makefile
  tests/optimizer/Makefile
  tests/physical/Makefile
  tests/storage/Makefile
  tests/sql/Makefile
  tests/analysis/Makefile
  tests/deploy/Makefile
  datagen/Makefile
])
AC_OUTPUT

