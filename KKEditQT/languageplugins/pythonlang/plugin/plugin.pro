TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = pythonlang.h
SOURCES         = pythonlang.cpp
TARGET          = $$qtLibraryTarget(pythonlang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/pythonlang
}
else {
	target.path = /tmp/langplugins/pythonlang
}

INSTALLS += target

CONFIG += install_ok

