TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = cpplang.h
SOURCES         = cpplang.cpp
TARGET          = $$qtLibraryTarget(cpplang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/cpplang
}
else {
	target.path = /tmp/langplugins/cpplang
}

INSTALLS += target

CONFIG += install_ok

