
AUTOMAKE_OPTIONS = subdir-objects

include ../flagsandlibs
include ../sources

kkeditqt_SOURCES = $(COMMONSRC)

pixfilesdir = $(pkgdatadir)/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

helpfilesdir = $(pkgdatadir)/help
helpfiles_DATA = ../resources/help/*

scriptfilesdir = $(pkgdatadir)/tools
scriptfiles_SCRIPTS = ../resources/tools/*

docfilesdir = $(pkgdatadir)/docs
docfiles_DATA = ../resources/docs/*

themesfilesdir = $(pkgdatadir)/themes
themesfiles_DATA = ../resources/themes/*

bin_PROGRAMS = kkeditqt

install-exec-hook:
	install -D ../resources/pixmaps/KKEditQTPlug.png $(DESTDIR)${prefix}/share/pixmaps/KKEditQTPlug.png
	install -D ../resources/pixmaps/KKEditQTPlugMenu.png $(DESTDIR)${prefix}/share/pixmaps/KKEditQTPlugMenu.png
	install -D ../resources/pixmaps/KKEditQT.png $(DESTDIR)${prefix}/share/pixmaps/KKEditQT.png
	install -D ../resources/pixmaps/KKEditQTProject.png $(DESTDIR)${prefix}/share/pixmaps/KKEditQTProject.png
	install -D ../resources/pixmaps/KKEditQT.desktop $(DESTDIR)${prefix}/share/applications/KKEditQT.desktop
	install -D ../resources/pixmaps/KKEditQTRoot.desktop $(DESTDIR)${prefix}/share/applications/KKEditQTRoot.desktop
	install -D ../resources/pixmaps/KKEditQT.png $(DESTDIR)${prefix}/share/icons/hicolor/128x128/apps/KKEditQT.png
	install -D ../resources/pixmaps/KKEditQTRoot.png $(DESTDIR)${prefix}/share/icons/hicolor/128x128/apps/KKEditQTRoot.png
	mkdir -vp $(DESTDIR)$(pkgdatadir)/plugins
	mkdir -vp "$(DESTDIR)$(pkgdatadir)/Example External Tools"
	cp -r "../resources/Example External Tools" "$(DESTDIR)$(pkgdatadir)"



