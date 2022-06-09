TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = SymbolsMenu.h
SOURCES         = SymbolsMenu.cpp
TARGET          = $$qtLibraryTarget(SymbolsMenu)
DESTDIR         = ../plugins

data.files = indicator-chars.txt
destdir = $$(INSTALLTO)
isEmpty(destdir) {
	LOCAL = 1
}

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/SymbolsMenu)
	target.path = ~/.KKEditQT/plugins/SymbolsMenu
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/plugins/SymbolsMenu
}

data.path = $${target.path}

INSTALLS += target
INSTALLS += data

CONFIG += install_ok

