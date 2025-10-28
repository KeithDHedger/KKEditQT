#
#
AC_INIT([KKEditQT],[1.6.2],[PROJ],[KKEditQT])

AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_CXX

PKG_PROG_PKG_CONFIG

X11_MODULES="x11 >= 1.0.0"
PKG_CHECK_MODULES(X11, $X11_MODULES)
AC_SUBST(X11_LIBS)

QTWIDGETS_MODULES="Qt6Widgets >= 6.6.0"
PKG_CHECK_MODULES(QTWIDGETS, $QTWIDGETS_MODULES)
AC_SUBST(QTWIDGETS_CFLAGS)
AC_SUBST(QTWIDGETS_LIBS)

QTCORE_MODULES="Qt6Core >= 6.6.0"
PKG_CHECK_MODULES(QTCORE,$QTCORE_MODULES)
AC_SUBST(QTCORE_CFLAGS)
AC_SUBST(QTCORE_LIBS)

QTPRINTSUPPORT_MODULES="Qt6PrintSupport >= 6.6.0"
PKG_CHECK_MODULES(QTPRINTSUPPORT, $QTPRINTSUPPORT_MODULES)
AC_SUBST(QTPRINTSUPPORT_CFLAGS)
AC_SUBST(QTPRINTSUPPORT_LIBS)

AC_DEFINE([_USEQT6_], [], [Build Qt6])

QTTERM_MODULES="qtermwidget6 >= 2.0.0"
PKG_CHECK_MODULES(QTTERM, $QTTERM_MODULES, [gotterm=yes], [:])
if test "$gotterm" = "yes"; then
	rm -f KKEditQT/plugins/TerminalPlugin/pass
else
	touch KKEditQT/plugins/TerminalPlugin/pass
	AC_MSG_NOTICE([No QTermWidget, not building TerminalPlugin])
fi
AC_MSG_NOTICE([Using Qt6])

AC_MSG_CHECKING(whether to build doc viewer)
AC_ARG_ENABLE(docviewer,AS_HELP_STRING([--enable-docviewer],[Build the Doc viewer]),
    [enable_docviewer=$enableval],
    [enable_docviewer="yes"])
AC_MSG_RESULT($enable_docviewer)

if test "$enable_docviewer" = "yes"; then
	WEBENGINEWIDGETS_MODULES="Qt6WebEngineWidgets >= 6.6.0"

	PKG_CHECK_MODULES(WEBENGINEWIDGETS, $WEBENGINEWIDGETS_MODULES)
	AC_SUBST(WEBENGINEWIDGETS_CFLAGS)
	AC_SUBST(WEBENGINEWIDGETS_LIBS)

	AC_DEFINE([_BUILDDOCVIEWER_], [], [Build doc viewer])
fi

AC_MSG_CHECKING(whether to include spell checker (aspell))
AC_ARG_ENABLE(aspell,AS_HELP_STRING([--enable-aspell],[Include spellchecker]),
    [enable_aspell=$enableval],
    [enable_aspell="yes"])
AC_MSG_RESULT($enable_aspell)
if test "$enable_aspell" = "yes"; then
	ASPELL_LIBS=-laspell
	AC_SUBST(ASPELL_LIBS)
	AC_DEFINE([_ASPELL_], [], [Include spellchecker])
fi

SQLITE_MODULES="sqlite3 >= 3.45.0"
PKG_CHECK_MODULES(SQLITE, $SQLITE_MODULES)
AC_SUBST(SQLITE_CFLAGS)
AC_SUBST(SQLITE_LIBS)

AC_MSG_CHECKING(whether to enable debug code)
AC_ARG_ENABLE(debugcode,AS_HELP_STRING([--enable-debugcode],[Debug code]),
    [enable_debugcode=$enableval],
    [enable_debugcode="no"])
AC_MSG_RESULT($enable_debugcode)
if test "$enable_debugcode" = "yes"; then
	AC_DEFINE([_DEBUGCODE_], [], [Debug code])
fi

AC_MSG_NOTICE(Checking for external programs ...)

AC_CHECK_PROG(CTAGSS, ctags ,true,false)
if test $CTAGSS = false; then
	AC_MSG_ERROR([No ctags program found!])
fi

AC_CHECK_PROG(PDFTOTEXT, pdftotext,true,false)
if test $PDFTOTEXT = false; then
	AC_MSG_NOTICE([No pdftotext program found!])
	AC_MSG_NOTICE([You wont be able to import pdf's until you install it ( part of the poppler pkg )])
	AC_MSG_NOTICE([You don't need to recompile after installing pdftotext )])
fi

AC_CHECK_PROG(PDFCROP, pdfcrop ,true,false)
if test $PDFCROP = false; then
	AC_MSG_NOTICE([No pdfcrop program found!])
	AC_MSG_NOTICE([PDF's wont set margins properly until you install it ( part of the texlive pkg )])
	AC_MSG_NOTICE([You don't need to recompile after installing pdfcrop )])
fi

AC_CHECK_PROG(DOXYGEN, doxygen ,true,false)
if test $DOXYGEN = false; then
	AC_MSG_NOTICE([No doxygen program found!])
	AC_MSG_NOTICE([Wont be able to build documentaion or docsets until you install it])
	AC_MSG_NOTICE([You don't need to recompile after installing doxygen )])
fi

AC_CHECK_PROG(QTSU, qtsu ,true,false)
if test $QTSU = false; then
	AC_MSG_NOTICE([No QtSu program found!])
	AC_MSG_NOTICE([You can download it from here:])
	AC_MSG_NOTICE([https://github.com/KeithDHedger/QtSu])
fi

AC_CHECK_PROG(MANPAGEEDITOR, manpageeditor ,true,false)
if test $MANPAGEEDITOR = false; then
	AC_MSG_NOTICE([No Manpage Editor program found!])
	AC_MSG_NOTICE([You can download it from here:])
	AC_MSG_NOTICE([https://github.com/KeithDHedger/ManPageEditor])
fi

AC_CONFIG_FILES(Makefile KKEditQT/app/Makefile KKEditQT/helpers/Makefile)
AC_OUTPUT


