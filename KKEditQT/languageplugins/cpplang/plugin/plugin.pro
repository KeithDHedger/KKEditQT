TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = cpplang.h
SOURCES         = cpplang.cpp
TARGET          = $$qtLibraryTarget(cpplang)
DESTDIR         = ../plugins

documentation.files = docs/*
destdir = $$(INSTALLTO)
isEmpty(destdir) {
	LOCAL = 1
}

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/cpplang)
	target.path = ~/.KKEditQT/langplugins/cpplang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/cpplang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok

