TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = htmllang.h
SOURCES         = htmllang.cpp
TARGET          = $$qtLibraryTarget(htmllang)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/htmllang)
	target.path = ~/.KKEditQT/langplugins/htmllang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/htmllang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


