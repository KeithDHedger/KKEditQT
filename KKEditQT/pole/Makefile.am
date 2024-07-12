CPPROGRAM=KKEditQTProgressBar

# expand pkg-config command before displaying
all:
	g++ -Wall $(QTWIDGETS_CFLAGS) $(QTWIDGETS_LIBS) -fPIC main.cpp -o $(CPPROGRAM)

install:
	mkdir -p $(DESTDIR)$(prefix)/bin
	cp $(CPPROGRAM) $(DESTDIR)${prefix}/bin/$(CPPROGRAM)

uninstall:
	rm $(DESTDIR)${prefix}/bin/$(CPPROGRAM)

clean:
	rm -f $(CPPROGRAM)

distclean: clean
	rm Makefile
