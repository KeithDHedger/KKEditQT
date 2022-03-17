TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = javalang.h
SOURCES         = javalang.cpp
TARGET          = $$qtLibraryTarget(javalang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/javalang
}
else {
	target.path = /tmp/langplugins/javalang
}

INSTALLS += target

CONFIG += install_ok

