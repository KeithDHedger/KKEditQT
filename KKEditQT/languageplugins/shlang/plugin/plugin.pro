TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = shlang.h
SOURCES         = shlang.cpp
TARGET          = $$qtLibraryTarget(shlang)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/shlang)
	target.path = ~/.KKEditQT/langplugins/shlang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/shlang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


