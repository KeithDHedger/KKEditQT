TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = shlang.h
SOURCES         = shlang.cpp
TARGET          = $$qtLibraryTarget(shlang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/shlang
}
else {
	target.path = /tmp/langplugins/shlang
}

INSTALLS += target

CONFIG += install_ok

