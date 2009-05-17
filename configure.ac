AC_PREREQ(2.57)
AC_INIT(eshutdown, 0.1, lunohod@openinkpot.org)
AM_INIT_AUTOMAKE(foreign)

AC_PROG_CC

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(ECORE, [ecore])
PKG_CHECK_MODULES(ECORE_CON, [ecore-con])
PKG_CHECK_MODULES(ECORE_EVAS, [ecore-evas])
PKG_CHECK_MODULES(EVAS, [evas])
PKG_CHECK_MODULES(EDJE, [edje])

AC_OUTPUT([Makefile src/Makefile themes/Makefile po/Makefile])
