TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = qt5toolkit.h
SOURCES         = qt5toolkit.cpp
TARGET          = $$qtLibraryTarget(qt5toolkit)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/toolkitplugins/qt5toolkit
}
else {
	target.path = /tmp/toolkitplugins/qt5toolkit
}

INSTALLS += target

CONFIG += install_ok



