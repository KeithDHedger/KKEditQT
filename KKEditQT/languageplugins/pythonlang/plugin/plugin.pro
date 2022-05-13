TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = pythonlang.h
SOURCES         = pythonlang.cpp
TARGET          = $$qtLibraryTarget(pythonlang)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/pythonlang)
	target.path = ~/.KKEditQT/langplugins/pythonlang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/pythonlang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


