
include ../flagsandlibs

MSGAPP=kkeditqtmsg
TAGREADERAPP=kkeditqttagreader
BARAPP=kkeditqtprogressbar

all:
	g++ $(MSGAPP).cpp -o $(MSGAPP)
	g++ $(TAGREADERAPP).cpp $(QTCORE_CFLAGS) $(QTCORE_LIBS) -fPIC -o $(TAGREADERAPP)
	g++ $(BARAPP).cpp $(QTWIDGETS_CFLAGS) $(QTWIDGETS_LIBS) -fPIC -o $(BARAPP)

install:
	mkdir -p $(DESTDIR)$(prefix)/bin
	cp $(MSGAPP) $(TAGREADERAPP) $(BARAPP) $(DESTDIR)${prefix}/bin/
	strip --strip-all $(DESTDIR)${prefix}/bin/$(MSGAPP)
	strip --strip-all $(DESTDIR)${prefix}/bin/$(TAGREADERAPP)
	strip --strip-all $(DESTDIR)${prefix}/bin/$(BARAPP)

uninstall:
	rm $(DESTDIR)${prefix}/bin/$(MSGAPP)
	rm $(DESTDIR)${prefix}/bin/$(TAGREADERAPP)
	rm $(DESTDIR)${prefix}/bin/$(BARAPP)

clean:
	rm -f $(MSGAPP)
	rm -f $(TAGREADERAPP)
	rm -f $(BARAPP)

distclean: clean
	rm Makefile
