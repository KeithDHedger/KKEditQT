TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport
HEADERS         = TerminalPlugin.h
SOURCES         = TerminalPlugin.cpp
TARGET          = $$qtLibraryTarget(TerminalPlugin)
DESTDIR         = plugins

documentation.files = docs/*
image.files = TerminalPlugin.png

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += data
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok

unix:!macx: LIBS += -lqtermwidget$$(USEQTVERSION)
