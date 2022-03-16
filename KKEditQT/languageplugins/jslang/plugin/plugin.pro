TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = jslang.h
SOURCES         = jslang.cpp
TARGET          = $$qtLibraryTarget(jslang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/jslang
}
else {
	target.path = /tmp/langplugins/jslang
}

INSTALLS += target

CONFIG += install_ok

