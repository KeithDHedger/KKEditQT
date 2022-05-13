TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = phplang.h
SOURCES         = phplang.cpp
TARGET          = $$qtLibraryTarget(phplang)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/phplang)
	target.path = ~/.KKEditQT/langplugins/phplang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/phplang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


