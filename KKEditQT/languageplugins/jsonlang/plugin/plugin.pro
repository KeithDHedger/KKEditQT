TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = jsonlang.h
SOURCES         = jsonlang.cpp
TARGET          = $$qtLibraryTarget(jsonlang)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/jsonlang)
	target.path = ~/.KKEditQT/langplugins/jsonlang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/jsonlang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


