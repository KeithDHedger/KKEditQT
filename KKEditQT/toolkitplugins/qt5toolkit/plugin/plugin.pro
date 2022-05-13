TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = qt5toolkit.h
SOURCES         = qt5toolkit.cpp
TARGET          = $$qtLibraryTarget(qt5toolkit)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/qt5toolkit)
	target.path = ~/.KKEditQT/toolkitplugins/qt5toolkit
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/toolkitplugins/qt5toolkit
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok




