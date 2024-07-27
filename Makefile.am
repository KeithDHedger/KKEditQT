
SUBDIRS = KKEditQT/app KKEditQT/helpers

all-am: plugs

plugs:
	@./buildplugs.sh build

install-plugs: plugs
	@./buildplugs.sh install $(DESTDIR)/${pkgdatadir}

clean-am:
	@./buildplugs.sh clean

install-exec-hook:
	@./buildplugs.sh install $(DESTDIR)/${pkgdatadir}
	chmod +x $(DESTDIR)$(pkgdatadir)/docs/makedocset
	strip --strip-all $(DESTDIR)${prefix}/bin/kkeditqt


remake:
	./remakesourcelist
	
uninstall-hook:
	rm -f $(DESTDIR)${prefix}/share/applications/KKEditQT.desktop $(DESTDIR)${prefix}/share/applications/KKEditQTRoot.desktop $(DESTDIR)${prefix}/share/pixmaps/KKEditQT.png $(DESTDIR)${prefix}/share/icons/hicolor/128x128/apps/KKEditQT.png $(DESTDIR)${prefix}/share/icons/hicolor/128x128/apps/KKEditQTRoot.png  $(DESTDIR)${prefix}/share/pixmaps/KKEditQTPlug.png $(DESTDIR)${prefix}/share/pixmaps/KKEditQTProject.png $(DESTDIR)${prefix}/share/pixmaps/KKEditQTPlugMenu.png
	rm -fr $(DESTDIR)$(pkgdatadir)

distclean-local:
	rm KKEditQT/src/moc_*


