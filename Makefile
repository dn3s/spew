CC = gcc
CFLAGS = -O3
DEPS = password.h
ODIR=obj
_OBJ = spew.o password.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))
INSTALL="install"
exec_prefix=$(prefix)
bindir=$(exec_prefix)/bin

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -I . $(CFLAGS)

spew: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm $(ODIR)/*; rm spew

.PHONY: install
install: spew
	$(INSTALL) spew $(DESTDIR)$(bindir)

uninstall:
	rm $(DESTDIR)$(bindir)/spew
