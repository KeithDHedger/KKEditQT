TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = makelang.h
SOURCES         = makelang.cpp
TARGET          = $$qtLibraryTarget(makelang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/makelang
}
else {
	target.path = /tmp/langplugins/makelang
}

INSTALLS += target

CONFIG += install_ok

