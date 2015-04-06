AC_PREREQ([2.69])
AC_INIT([amplify], [0.1], [dnk1618@gmail.com])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_HEADERS([src/build-config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])

AM_INIT_AUTOMAKE([subdir-objects foreign tar-pax])

# If automake supports "silent rules", enable them by default
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

#
# Compiler flags
#
CXXFLAGS="-std=c++11 -fno-rtti -Werror -Wformat=2 $CXXFLAGS"

#
# Additional CXXFLAGS used when --enable-debug-build is used
#
CXXFLAGS_DEBUG_ENABLED="-g -O0"

#
# Additional CXXFLAGS used when --disable-debug-build is used
# and when --enable-debug-build is
#
# Don't add -DNDEBUG here. It's added to the generated config.h
# header file.
#
CXXFLAGS_DEBUG_DISABLED="-Os -fomit-frame-pointer -ffunction-sections -fdata-sections "

#
#
#
LDFLAGS_DEBUG_DISABLED="-Wl,--gc-sections $LDFLAGS"

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL
AC_PROG_LIBTOOL

# Checks for libraries.
#PKG_CHECK_MODULES([glibmm], [glibmm-2.4 > 0], [],
#                  AC_MSG_FAILURE([glibmm-2.4 development files not found!]))

########################
#      PThreads        #
########################
pthread_CFLAGS=error
pthread_LIBS=error
SAVE_LIBS=$LIBS

if test "x$pthread_LIBS" = "xerror"; then
   AC_CHECK_LIB(c_r, pthread_attr_init, [
                     pthread_CFLAGS="-D_THREAD_SAFE -pthread"
                     pthread_LIBS="-pthread" ])
fi

if test "x$pthread_LIBS" = "xerror"; then
   AC_CHECK_LIB(pthread, pthread_attr_init, [
                         pthread_CFLAGS="-D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS"
                         pthread_LIBS="-lpthread" ])
fi

if test "x$pthread_LIBS" = "xerror"; then
   AC_CHECK_LIB(pthreads, pthread_attr_init, [
                          pthread_CFLAGS="-D_THREAD_SAFE"
                          pthread_LIBS="-lpthreads" ])
fi

if test "x$pthread_LIBS" = "xerror"; then
   AC_MSG_FAILURE(pthread library is missing)
fi

LIBS=$SAVE_LIBS
AC_SUBST(pthread_CFLAGS)
AC_SUBST(pthread_LIBS)

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h limits.h locale.h netdb.h stdint.h stdlib.h string.h sys/time.h syslog.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([dup2 gettimeofday localtime_r memset mkdir regcomp setlocale strerror strstr])

# ./configure script options
AC_ARG_ENABLE([debug-build],
     [AC_HELP_STRING([--enable-debug-build], [enable debugging flags (default=no)])],
     [case "${enableval}" in
       yes) debug=yes ;;
       no)  debug=no ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug-build]) ;;
     esac], [debug=no])

if test "$debug" = "yes"; then
   CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG_ENABLED"
else
   CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG_DISABLED"
   LDFLAGS="$LDFLAGS $LDFLAGS_DEBUG_DISABLED"
   AC_DEFINE([NDEBUG], [1], [Debug build disabled])
#   AC_DEFINE([QUEX_OPTION_COMPUTED_GOTOS], [1], [Enables/disables the usage of computed gotos])
fi

# ./configure script options
AC_ARG_ENABLE([logging],
     [AC_HELP_STRING([--enable-logging], [enable logging (default=yes)])],
     [case "${enableval}" in
       yes) logging=yes ;;
       no)  logging=no ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-logging]) ;;
     esac], [logging=yes])

if test "$logging" = "yes"; then
   AC_DEFINE([AMPUSH_LOGGING_ENABLED], [1], [Enabled logging support in AmPush])
fi

AC_DEFINE([AMPUSH_COMPILE_FLAGS], [$CXXFLAGS], [Compilation flags])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
