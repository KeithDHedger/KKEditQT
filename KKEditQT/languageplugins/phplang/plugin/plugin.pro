TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = phplang.h
SOURCES         = phplang.cpp
TARGET          = $$qtLibraryTarget(phplang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/phplang
}
else {
	target.path = /tmp/langplugins/phplang
}

INSTALLS += target

CONFIG += install_ok

