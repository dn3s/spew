CC = gcc
CFLAGS = -I .
DEPS = password.h
ODIR=obj
_OBJ = spew.o password.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

spew: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
