TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = makelang.h
SOURCES         = makelang.cpp
TARGET          = $$qtLibraryTarget(makelang)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/makelang)
	target.path = ~/.KKEditQT/langplugins/makelang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/makelang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


