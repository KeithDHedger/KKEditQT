TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = jslang.h
SOURCES         = jslang.cpp
TARGET          = $$qtLibraryTarget(jslang)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/jslang)
	target.path = ~/.KKEditQT/langplugins/jslang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/jslang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


