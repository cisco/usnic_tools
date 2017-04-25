#
# Copyright (c) 2016-2017, Cisco Systems, Inc. All rights reserved.
#
# This software is available to you under a choice of one of two
# licenses.  You may choose to be licensed under the terms of the GNU
# General Public License (GPL) Version 2, available from the file
# COPYING in the main directory of this source tree, or the
# BSD license below:
#
#     Redistribution and use in source and binary forms, with or
#     without modification, are permitted provided that the following
#     conditions are met:
#
#      - Redistributions of source code must retain the above
#	 copyright notice, this list of conditions and the following
#	 disclaimer.
#
#      - Redistributions in binary form must reproduce the above
#	 copyright notice, this list of conditions and the following
#	 disclaimer in the documentation and/or other materials
#	 provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

AC_PREREQ([2.57])
AC_INIT([usnic-tools], [1.1.2.1a1])

AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE([foreign no-define 1.11 dist-bzip2])

dnl SILENT_RULES is new in AM 1.11, but we require 1.11 or higher via
dnl autogen.  Limited testing shows that calling SILENT_RULES directly
dnl works in more cases than adding "silent-rules" to INIT_AUTOMAKE
dnl (even though they're supposed to be identical).  Shrug.
AM_SILENT_RULES([yes])

AC_PROG_CC_C99
AS_IF([test "$ac_cv_prog_cc_c99" = "no"],
      [AC_MSG_WARN([C99 compiler unable to be found.])
       AC_MSG_ERROR([Cannot continue])])

LT_INIT

AC_ARG_WITH([libfabric],
	    AC_HELP_STRING([--with-libfabric], [Use non-default libfabric location - default NO]))

AS_IF([test "$with_libfabric" = "yes"],
      [with_libfabric=])
AS_IF([test "$with_libfabric" = "no"],
      [AC_MSG_WARN([Cannot specify --without-libfabric])
       AC_MSG_ERROR([Cannot continue])])
AS_IF([test "x$with_libfabric" != "x"],
      [include_happy=0
       lib_happy=0
       AS_IF([test -d "$with_libfabric/include"], [include_happy=1])
       AS_IF([test -d "$with_libfabric/lib64"],
             [fab_libdir=lib64
	      lib_happy=1])
       AS_IF([test -d "$with_libfabric/lib"],
             [fab_libdir=lib
	      lib_happy=1])
       AS_IF([test $include_happy -eq 0 || test $lib_happy -eq 0],
             [AC_MSG_WARN([Could not find include or lib dir in $with_libfabric])
	      AC_MSG_ERROR([Cannot continue])])

       CPPFLAGS="-I$with_libfabric/include $CPPFLAGS"
       LDFLAGS="-L$with_libfabric/$fab_libdir $LDFLAGS"
      ])

dnl Checks for libraries
AC_CHECK_LIB([fabric], fi_getinfo, [],
	     AC_MSG_ERROR([fi_getinfo() not found.  usnic-tools requires libfabric.]))

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADER([rdma/fabric.h], [],
		[AC_MSG_ERROR([<rdma/fabric.h> not found.  usnic-tools requires libfabric.])])
AC_CHECK_HEADER([rdma/fi_ext_usnic.h], [],
		[AC_MSG_WARN([libfabric does not appear to have usNIC support included.])
		 AC_MSG_ERROR([Cannot continue])])

dnl Make sure we have a new enough libfabric
AC_MSG_CHECKING([if libfabric >=v1.3])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <rdma/fabric.h>
#if !defined(FI_MAJOR_VERSION) || !defined(FI_MINOR_VERSION)
#error cannot figure out libfabric version
#elif FI_MAJOR_VERSION < 1
#error non-sensical libfabric version (MAJOR < 1)
#elif FI_MAJOR_VERSION == 1 && FI_MINOR_VERSION < 3
#error libfabric version too old (<1.3)
#endif
]], [])],
    [AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])
     AC_MSG_WARN([usnic-tools requires libfabric >=v1.3])
     AC_MSG_ERROR([Cannot continue])])

# See if RPATH support works
LDFLAGS_save=$LDFLAGS
LDFLAFGS="$LDFLAGS_save -Wl,-rpath -Wl,$fab_libdir"
AC_CHECK_LIB([fabric], fi_version,
	     [rpath_works=1],
	     [rpath_works=0
	      LDFLAGS=$LDFLAGS_save])

# If RPATH works, see if RUNPATH works, too
AS_IF([test $rpath_works -eq 1],
    [LDFLAGS_save2=$LDFLAGS
     LDFLAGS="$LDFLAGS_save -Wl,-rpath -Wl,$fab_libdir -Wl,--enable-new-dtags"
     AC_CHECK_LIB([fabric], fi_log, [],
                   [LDFLAGS=$LDFLAGS_save2])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
