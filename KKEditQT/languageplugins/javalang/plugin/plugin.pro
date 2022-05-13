TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = javalang.h
SOURCES         = javalang.cpp
TARGET          = $$qtLibraryTarget(javalang)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/javalang)
	target.path = ~/.KKEditQT/langplugins/javalang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/javalang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


