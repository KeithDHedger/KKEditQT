CPPROGRAM=kkeditqtmsg

all:
	g++ main.cpp -o $(CPPROGRAM)

install:
	mkdir -p $(DESTDIR)$(prefix)/bin
	cp $(CPPROGRAM) $(DESTDIR)${prefix}/bin/$(CPPROGRAM)

uninstall:
	rm $(DESTDIR)${prefix}/bin/$(CPPROGRAM)

clean:
	rm -f $(CPPROGRAM)