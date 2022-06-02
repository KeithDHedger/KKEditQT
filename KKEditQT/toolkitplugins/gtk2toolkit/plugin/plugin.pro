TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = gtk2toolkit.h
SOURCES         = gtk2toolkit.cpp
TARGET          = $$qtLibraryTarget(gtk2toolkit)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/gtk2toolkit)
	target.path = ~/.KKEditQT/toolkitplugins/gtk2toolkit
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/toolkitplugins/gtk2toolkit
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok

