
SUBDIRS = KKEditQT/app

all-am: plugs

plugs:
	./buildplugs.sh build $(DESTDIR)/${pkgdatadir}

#install-plugs: plugs
#	./buildplugs.sh install $(DESTDIR)/${pkgdatadir}

clean-am:
	./buildplugs.sh clean

install-exec-hook:
	./buildplugs.sh install $(DESTDIR)/${pkgdatadir}

remake:
	$(shell ./remakesourcelist)
	
uninstall-hook:
	rm -f $(DESTDIR)${prefix}/share/applications/KKEditQT.desktop $(DESTDIR)${prefix}/share/applications/KKEditQTRoot.desktop $(DESTDIR)${prefix}/share/pixmaps/KKEditQT.png $(DESTDIR)${prefix}/share/icons/hicolor/256x256/apps/KKEditQT.png $(DESTDIR)${prefix}/share/icons/hicolor/256x256/apps/KKEditQTRoot.png  $(DESTDIR)${prefix}/share/pixmaps/KKEditQTPlug.png
	rm -fr $(pkgdatadir)


